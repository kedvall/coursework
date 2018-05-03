#include "lab56pkg/lab56.h"

#define DEBUG false

extern ImageConverter* ic_ptr; //global pointer from the lab56.cpp

// Magic numbers
#define LEFT 1
#define RIGHT 2
#define UP 3
#define DOWN 4
#define CROSSHAIR_PIXEL -420
#define SPIN_RATE 20  /* Hz */

bool isReady=1;
bool pending=0;

float SuctionValue = 0.0;

bool leftclickdone = 1;
bool rightclickdone = 1;

// Initialize arrays for centroid-finding
int centroidRows[5000] = {0};
int centroidCols[5000] = {0};
int found_centroids = 0;
float x_world = 0;
float y_world = 0;
float z_world = 0;

/*****************************************************
* Functions in class:
* **************************************************/
int getLeftPixel(int pixellabel[][640], int row, int col);
int getAbovePixel(int pixellabel[][640], int row, int col);
int noiseEliminate(int pixellabel[][640], int num_rows, int num_cols,int num_labels);
int findCentroids(int pixellabel[][640], int num_rows, int num_cols, int num_objects, int centroidRows[], int centroidCols[]);
void drawCrosshairs(int pixellabel[][640], int num_rows, int num_cols, int centroidX[], int centroidY[], int num_objects);
void translate_to_world_coords(float row_in_pixels, float col_in_pixels, float *x_world, float *y_world);
void find_wc(float row_in_pixels, float col_in_pixels);
int find_nearest_centroid(float row_in_pixels, float col_in_pixels);

//constructor(don't modify)
ImageConverter::ImageConverter():it_(nh_)
{
    // Subscrive to input video feed and publish output video feed
    image_sub_ = it_.subscribe("/cv_camera_node/image_raw", 1,
    	&ImageConverter::imageCb, this);
    image_pub_ = it_.advertise("/image_converter/output_video", 1);
    namedWindow(OPENCV_WINDOW);
    pub_command=nh_.advertise<ece470_ur3_driver::command>("ur3/command",10);
    sub_position=nh_.subscribe("ur3/position",1,&ImageConverter::position_callback,this);

	sub_io_states=nh_.subscribe("ur_driver/io_states",1,&ImageConverter::suction_callback,this);

	srv_SetIO = nh_.serviceClient<ur_msgs::SetIO>("ur_driver/set_io");   // succ

    set_suction(false);

    driver_msg.destination=lab_invk(0,0,0.2,0);
    move_robot();
}

//destructor(don't modify)
ImageConverter::~ImageConverter()
{
    cv::destroyWindow(OPENCV_WINDOW);
}

void ImageConverter::position_callback(const ece470_ur3_driver::positions::ConstPtr& msg)
{
	isReady=msg->isReady;
	pending=msg->pending;
}

void ImageConverter::suction_callback(const ur_msgs::IOStates::ConstPtr& msg)
{
	SuctionValue = msg->analog_in_states[0].state;
}


//subscriber callback function, will be called when there is a new image read by camera
void ImageConverter::imageCb(const sensor_msgs::ImageConstPtr& msg)
{
    cv_bridge::CvImagePtr cv_ptr;
    try
    {
      cv_ptr = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::BGR8);
    }
    catch (cv_bridge::Exception& e)
    {
      ROS_ERROR("cv_bridge exception: %s", e.what());
      return;
    }
    // create an gray scale version of image
    Mat gray_image;
	cvtColor( cv_ptr->image, gray_image, CV_BGR2GRAY );
    // convert to black and white img, then associate objects:

	Mat bw_image;
	adaptiveThreshold(gray_image,bw_image,255,0,0,151,5);
	//adaptiveThreshold(scr,dst,MAXVALUE,adaptiveMethod,thresholdType,blocksize,C);
	//adaptiveMethod = 0, ADAPTIVE_THRESH_MEAN_C
	//thresholdType = 0, BINARY
	//blocksize
	//C constant subtracted from tz.

// FUNCTION you will be completing
    Mat associate_image = associateObjects(bw_image); // find associated objects

    // Update GUI Window
    imshow("Image window", cv_ptr->image);
    imshow("gray_scale", gray_image);
    imshow("black and white", bw_image);
    imshow("associate objects", associate_image);
    waitKey(3);
    // Output some video stream
    image_pub_.publish(cv_ptr->toImageMsg());
}

/*****************************************************
	 * Function for Lab 5
* **************************************************/
// Take a grayscale image as input and return an thresholded image.
// You will implement your algorithm for calculating threshold here.
Mat ImageConverter::thresholdImage(Mat gray_img)
{
		int   totalpixels;
		Mat bw_img  = gray_img.clone(); // copy input image to a new image
		totalpixels	  = gray_img.rows*gray_img.cols;			// total number of pixels in image
		uchar graylevel; // use this variable to read the value of a pixel
		int zt=0; // threshold grayscale value

		zt = 100;  // you will be finding this automatically

		//std::cout<<zt<<std::endl;
		// threshold the image
		for(int i=0; i<totalpixels; i++)
		{
			graylevel = gray_img.data[i];
			if(graylevel>zt) bw_img.data[i]= 255; // set rgb to 255 (white)
			else             bw_img.data[i]= 0; // set rgb to 0   (black)
		}
	return bw_img;
}
/*****************************************************
	 * Function for Lab 5
* **************************************************/
// Take an black and white image and find the object it it, returns an associated image with different color for each image
// You will implement your algorithm for rastering here
Mat ImageConverter::associateObjects(Mat bw_img)
{
    // Labels for numbers
    int label_len = 4200;
    const int white = 255; const int black = 0;

	//initiallize the variables you will use
	int height,width; // number of rows and colums of image
	int red, green, blue; //used to assign color of each objects
	uchar pixel; //used to read pixel value of input image
	height = bw_img.rows;
	width = bw_img.cols;
	int num = 0;

	// this create a 2 dimensional array pixellabel[row][col]
    int label[label_len];
    int *equiv[label_len];
    int pixellabel[height][640]; //width];

    // Equate the equiv table to the initial labels
    for (int i=0; i<label_len; i++)
        equiv[i] = &label[i];

    // Set pixellabel as background or foreground
    for (int cur_row=0; cur_row<height; cur_row++) {
        for (int cur_col=0; cur_col<width; cur_col++) {
            if (bw_img.data[cur_row*bw_img.cols + cur_col]  == white)
                pixellabel[cur_row][cur_col] = -1; // Set as background
            else
                pixellabel[cur_row][cur_col] = 0; // Set as foreground
        }
    }

    //**************************************************************************
    // First raster scan
    //**************************************************************************
    int labelnum = 1;
    for (int cur_row=0; cur_row<height; cur_row++) {
        for (int cur_col=0; cur_col<width; cur_col++) {
            int cur_pixel = pixellabel[cur_row][cur_col];
            int left_pixel = getLeftPixel(pixellabel, cur_row, cur_col);
            int above_pixel = getAbovePixel(pixellabel, cur_row, cur_col);
            if (cur_pixel == 0)   // check if the pixel represents an object
            {
                if ((left_pixel == -1) && (above_pixel == -1))  // both are background
                {
                    pixellabel[cur_row][cur_col] = labelnum;
                    label[labelnum] = labelnum;
                    labelnum ++;
                }
                else if ((left_pixel != -1) && (above_pixel == -1))  // only left is object
                {
                    pixellabel[cur_row][cur_col] = left_pixel;
                }
                else if ((left_pixel == -1) && (above_pixel != -1))  // only above is object
                {
                    pixellabel[cur_row][cur_col] = above_pixel;
                }
                else if ((left_pixel != -1) && (above_pixel != -1))  // both left and above are object
                {
                    int minPixel;
                    int maxPixel;
                    if (equiv[left_pixel] < equiv[above_pixel])
                    {
                        minPixel = left_pixel;
                        maxPixel = above_pixel;
                    } else {
                        minPixel = above_pixel;
                        maxPixel = left_pixel;
                    }
//                    cout << "Min Pixel is " << minPixel << endl;
//                    cout << "Max Pixel is " << maxPixel << endl;

                    pixellabel[cur_row][cur_col] = minPixel;
                    *equiv[maxPixel] = *equiv[minPixel];
                    equiv[maxPixel] = equiv[minPixel];
                }
            }
        }
    }

    //**************************************************************************
    // Second raster scan
    //**************************************************************************
    for (int cur_row=0; cur_row<height; cur_row++) {
        for (int cur_col=0; cur_col<width; cur_col++) {
            int cur_pixel = pixellabel[cur_row][cur_col];
            if (cur_pixel != -1)
                pixellabel[cur_row][cur_col] = *equiv[cur_pixel];
        }
    }

    // Eliminate noise, and find the number of final objects
    int num_objects = noiseEliminate(pixellabel, height, width, labelnum);

    // Find the centroids.
    found_centroids = findCentroids(pixellabel, height, width, num_objects, centroidRows, centroidCols);

    // Draw crosshairs
    drawCrosshairs(pixellabel, height, width, centroidCols, centroidRows, found_centroids);

	// assign UNIQUE color to each object
	Mat associate_img = Mat::zeros( bw_img.size(), CV_8UC3 ); // function will return this image
	Vec3b color;
	for(int row=0; row<height; row++)
	{
		for(int col=0; col<width; col++)
		{
			switch (  pixellabel[row][col] )
			{
				case 0:
					red    = 128; // you can change color of each objects here
					green = 128;
					blue   = 128;
					break;
				case 1:
					red    = 255; // you can change color of each objects here
					green  = 0;
					blue   = 0;
					break;
				case 2:
					red    = 0;
					green  = 255;
					blue   = 0;
					break;
				case 3:
					red    = 0;
					green  = 0;
					blue   = 255;
					break;
				case 4:
					red    = 255;
					green  = 255;
					blue   = 0;
					break;
				case 5:
					red    = 255;
					green  = 0;
					blue   = 255;
					break;
				case 6:
					red    = 0;
					green  = 255;
					blue   = 255;
					break;
                case 7:
                    red    = 128;
                    green  = 128;
                    blue   = 0;
                    break;
                case 8:
                    red    = 128;
                    green  = 0;
                    blue   = 128;
                    break;
                case 9:
                    red    = 0;
                    green  = 128;
                    blue   = 128;
                 	break;
                case CROSSHAIR_PIXEL:
                    red    = 255;
                    green  = 255;
                    blue   = 255;
                    break;
				default:
					red    = 0;
					green = 0;
					blue   = 0;
					break;
			}

			color[0] = blue;
			color[1] = green;
			color[2] = red;
			associate_img.at<Vec3b>(Point(col,row)) = color;
		}
	}

	return associate_img;
}


/*****************************************************
	*Function for Lab 6
 * **************************************************/
 //This is a call back function of mouse click, it will be called when there's a click on the video window.
 //You will write your coordinate transformation in onClick function.
 //By calling onClick, you can use the variables calculated in the class function directly and use publisher
 //initialized in constructor to control the robot.
 //lab4 and lab3 functions can be used since it is included in the "lab4.h"
void onMouse(int event, int x, int y, int flags, void* userdata)
{
    // cout << "Entered " << __func__ << endl;

		ic_ptr->onClick(event,x,y,flags,userdata);
}

void ImageConverter::onClick(int event,int x, int y, int flags, void* userdata)
{
    // cout << "Entered " << __func__ << endl;
    float row_in_pixels, col_in_pixels;
	// For use with Lab 6
	// If the robot is holding a block, place it at the designated row and column.
	if  ( event == EVENT_LBUTTONDOWN ) //if left click, do nothing other than printing the clicked point
	{
		if (rightclickdone == 1 && leftclickdone == 1) {
			leftclickdone = 0;  // code started
			// ROS_INFO_STREAM("left click:  (" << x << ", " << y << ")");  //the point you clicked

            row_in_pixels = (float)y;
            col_in_pixels = (float)x;
            cout << "left click! Row, Col in pixels: (" << row_in_pixels << ", " << col_in_pixels << ")" << endl;

			// put your left click code here
            // int nearest_centroid = find_nearest_centroid(row_in_pixels, col_in_pixels);
            // // row_in_pixels = centroidRows[nearest_centroid];
            // // col_in_pixels = centroidCols[nearest_centroid];
            // cout << "Using centroid " << nearest_centroid << " with coords row: " << row_in_pixels << " col: " << col_in_pixels << endl;

            find_wc(row_in_pixels, col_in_pixels);

            //LAB4's lab_invk() this gives the joint angles for the chosen point
            //kevin
            cout << "Publishing: " << x_world << " " << y_world << " " << z_world << endl;
            driver_msg.destination = lab_invk(x_world, y_world, z_world+0.1, 0);
            move_robot();
            driver_msg.destination = lab_invk(x_world, y_world, z_world, 0);
            move_robot();
            set_suction(true);
            driver_msg.destination = lab_invk(x_world, y_world, z_world+0.1, 0);
            move_robot();
            if (SuctionValue < 2.0)
            {
                cout << "Error! Block not picked up." << endl;
                set_suction(false);
            }

			leftclickdone = 1; // code finished
		} else {
			ROS_INFO_STREAM("Previous click not finshed, IGNORING this Click");
		}
	}
	else if  ( event == EVENT_RBUTTONDOWN )//if right click, find nearest centroid,
	{
		if (rightclickdone == 1 && leftclickdone == 1) {  // if previous clicks not finished ignore

            if (SuctionValue < 2.0)
                cout << "Error! No block currently held" << endl;

			rightclickdone = 0;  // starting code
			ROS_INFO_STREAM("right click:  (" << x << ", " << y << ")");  //the point you clicked

			// put your right click code here
            row_in_pixels = (float)y;
            col_in_pixels = (float)x;
            cout << "right click! Row, Col in pixels: (" << row_in_pixels << ", " << col_in_pixels << ")" << endl;
            find_wc(row_in_pixels, col_in_pixels);

            cout << "Publishing: " << x_world << " " << y_world << " " << z_world << endl;
            driver_msg.destination = lab_invk(x_world, y_world, z_world+0.1, 0);
            move_robot();
            driver_msg.destination = lab_invk(x_world, y_world, z_world, 0);
            move_robot();
            set_suction(false);
            driver_msg.destination = lab_invk(x_world, y_world, z_world+0.1, 0);
            move_robot();

			rightclickdone = 1; // code finished
		} else {
			ROS_INFO_STREAM("Previous click not finshed, IGNORING this Click");
		}
	}
}


// Returns value of left neighbor if valid
// If there's no valid neighbor, return -1
int getLeftPixel(int pixellabel[][640], int row, int col)
{
    // Check if we are operating on the top row or the leftmost column
    if (col != 0)
        return pixellabel[row][col-1];
    else
        return -1;
}


// Returns value of above neighbor if valid
// If there's no valid neighbor, return -1
int getAbovePixel(int pixellabel[][640], int row, int col)
{
    // Check if we are operating on the top row or the leftmost column
    if (row != 0)
        return pixellabel[row-1][col];
    else
        return -1;
}


// Noise Elimination function that moves through pixellabel and counts instacnces of each label
// labels with count lower than the noise_threshold will be set back to White (-1)
// takes in pixellabel, its dimensions and total number of labels we assigned to objects
// returns the final number of objects in the image, and modifies pixellabel in place.
int noiseEliminate(int pixellabel[][640], int num_rows, int num_cols, int num_labels)
{
    // cout << "Entered " << __func__ << endl;
    // cout << "Labels " << num_labels << endl;
    // Counts the number of pixels in each label in pixellabel
    // if below pixel_threshold, converts it to white (-1)
    // Otherwise leaves it alone
    int lower_noise_threshold = 750;
    int upper_noise_threshold = 2700;
    int num_matching_pixels[num_labels];
    int color_label = 0;

    // Initialize pixellabel to 0
    for (int i=0; i<num_labels; i++)
        num_matching_pixels[i] = 0;

    // Count number of pixels in each grouping
    for (int row = 0; row < num_rows; row++) {
        for (int col = 0; col < num_cols; col++) {
            if (pixellabel[row][col] != -1)
                num_matching_pixels[pixellabel[row][col]]++;
        }
    }

    // Now step through the image to find labels below noise_threshold
    for (int i = 0; i < num_labels; i++) {
        // Check if the number of pixels is outside the valid bounds
        if (num_matching_pixels[i] < lower_noise_threshold || num_matching_pixels[i] > upper_noise_threshold)
            num_matching_pixels[i] = -1; //Mark this as invalid
        else {
            // cout << "Object " << color_label << " size is: " << num_matching_pixels[i] << endl;
            num_matching_pixels[i] = color_label++;
        }
    }

    // Now go and set pixels outside of valid bounds to -1, or their respective color
    for (int row = 0; row < num_rows; row++) {
        for (int col = 0; col < num_cols; col++) {
            if (pixellabel[row][col] != -1)
                pixellabel[row][col] = num_matching_pixels[pixellabel[row][col]];
        }
    }

    return color_label;
}


// Function that iterates through pixellabel and finds the centroids of all objects in the array.
// Places centroid coordinates in the parallel centroidX and centroidY arrays, where the index corresponds
// to the object number.
// Returns the number of centroids found
int findCentroids(int pixellabel[][640], int num_rows, int num_cols, int num_objects, int centroidRows[], int centroidCols[])
{
    // cout << "Entered " << __func__ << endl;
    // Another int to count how many centroids we created
    int found_centroids = 0;
    // Iterate through every object to find their centroid
    for (int object=0; object<num_objects; object++)
    {
        // For each object, iterate through pixellabel and add up the rows and columns of that object's pixels
        unsigned long int rowSum = 0, colSum = 0, totalPixels = 0;
        for (int row = 0; row < num_rows; row++)
        {
            for (int col = 0; col < num_cols; col++)
            {
                if (pixellabel[row][col] == object)
                {
                    rowSum += row;
                    colSum += col;
                    totalPixels += 1;
                }
            }
        }
        // Now, divide the row and column sums by the total number of pixels to find the centroid row and column
        if (totalPixels > 750)
        {
            // cout << "rowsum: " << rowSum << " colsum: " << colSum << " totalpixels: " << totalPixels << endl;
            // cout << "dividing rowsum: " << rowSum << " by totalpixels: " << totalPixels << " gives: " << rowSum/totalPixels << endl;
            // cout << "dividing colsum: " << rowSum << " by totalpixels: " << totalPixels << " gives: " << colSum/totalPixels << endl;
            centroidRows[object] = rowSum/totalPixels;
            centroidCols[object] = colSum/totalPixels;
            found_centroids++;
        }
        else
            cout << "too few pixels " << endl;
    }
    return found_centroids;
}


int drawLine(int pixellabel[][640], int num_rows, int num_cols, int cur_col, int cur_row, int direction)
{
    if (DEBUG) cout << "Entered " << __func__ << endl;

    // Draw left
    if (direction == LEFT) {
        if (DEBUG) cout << "In left" << endl;
        for (int i=0; i<25; i++) {
            if (DEBUG) cout << "Current col: " << cur_col << endl;
            if (DEBUG) cout << "Current row: " << cur_row << endl;
            if (cur_col<0)
                break;
            pixellabel[cur_row][cur_col] = CROSSHAIR_PIXEL;
            cur_col--;
        }
    // Draw right
    } else if (direction == RIGHT) {
        if (DEBUG) cout << "In right" << endl;
        for (int i=0; i<25; i++) {
            if (DEBUG) cout << "Current col: " << cur_col << endl;
            if (DEBUG) cout << "Current row: " << cur_row << endl;
            if (cur_col>num_cols)
                break;
            pixellabel[cur_row][cur_col] = CROSSHAIR_PIXEL;
            cur_col++;
        }
    // Draw up
    } else if (direction == UP) {
        if (DEBUG) cout << "In up" << endl;
        for (int i=0; i<25; i++) {
            if (DEBUG) cout << "Current col: " << cur_col << endl;
            if (DEBUG) cout << "Current row: " << cur_row << endl;
            if (cur_row<0)
                break;
            pixellabel[cur_row][cur_col] = CROSSHAIR_PIXEL;
            cur_row--;
        }
    // Draw down
    } else if (direction == DOWN) {
        if (DEBUG) cout << "In down" << endl;
        for (int i=0; i<25; i++) {
            if (DEBUG) cout << "Current col: " << cur_col << endl;
            if (DEBUG) cout << "Current row: " << cur_row << endl;
            if (cur_row>num_rows)
                break;
            pixellabel[cur_row][cur_col] = CROSSHAIR_PIXEL;
            cur_row++;
        }
    }
}


// Go through centroid array and draw crosshairs on each object
void drawCrosshairs(int pixellabel[][640], int num_rows, int num_cols, int centroidX[], int centroidY[], int num_objects)
{
    if (DEBUG) cout << "Entered " << __func__ << endl;
    for (int object=0; object<num_objects; object++) {
        drawLine(pixellabel, num_rows, num_cols, centroidX[object], centroidY[object], LEFT);
        drawLine(pixellabel, num_rows, num_cols, centroidX[object], centroidY[object], RIGHT);
        drawLine(pixellabel, num_rows, num_cols, centroidX[object], centroidY[object], UP);
        drawLine(pixellabel, num_rows, num_cols, centroidX[object], centroidY[object], DOWN);
    }
}


// Find the world coordinates given the camera r and c
void find_wc(float row_in_pixels, float col_in_pixels) {
    // cout << "Entered " << __func__ << endl;
    // Constants to be measured
    float theta = PI - 0.0778226; // adjust as neces ary +/-
    float Or = 480/2, Oc = 640/2; // half the size of the image
    float Tx = 20.6711, Ty =13.9597; // Find from image (click on the world origin in image)
    float B = 7.45; // calc this (ratio of pixel size to actual size of an object)

    // calculating xc and yc (the coordinate in the camera frame with unit 'cm')
    float xc = ((float)row_in_pixels-Or)/B; // xc is just a intermediate variable
    float yc = ((float)col_in_pixels-Oc)/B; // xy is just a intermediate variable
    // Verified that image frame has origin in midpoint of image array
    // Verified that scaling to cm is correct
    // xc,yc is the coordinate in the camera frame with unit 'cm'

    // cout << "Input coords row_in_pixels: " << row_in_pixels << " col_in_pixels: " << col_in_pixels << endl;
    // cout << "xc: " << xc << " yc: " << yc << endl;
    // converting xc,yc into xw,yw unit 'cm'
    x_world = cos(theta)*(xc - Tx) + sin(theta)*(yc - Ty);
    x_world = x_world/100;
    y_world = sin(theta)*(Tx - xc) + cos(theta)*(yc - Ty);
    y_world = y_world/100;

    z_world = 3.0/100; // This is a constant and equal to the height of the cube in 'cm'
    // Print for verification
    cout << "x_world: " << x_world << " y_world: " << y_world << " z_world: " << z_world << endl;
}


int find_nearest_centroid(float row_in_pixels, float col_in_pixels) {
    float min_distance = 640.0+480.0; // Shouldn't be able to get higher than this (or even get to this)
    float new_distance = 640.0+480.0;
    int centroid = -1;

    for (int i=0; i<found_centroids; i++) {
        new_distance = sqrt( pow(row_in_pixels-centroidRows[i],2) + pow(col_in_pixels-centroidCols[i],2) );
        if (new_distance < min_distance) {
            min_distance = new_distance;
            centroid = i;
        }
    }

    return centroid;
}


// Helper Function to move the robot to given world coordinates.
// Call after setting the driver_msg
void ImageConverter::move_robot()
{
    //publish the point to the robot
    ros::Rate loop_rate(SPIN_RATE); // Initialize the rate to publish to ur3/command
    int spincount = 0;
    driver_msg.duration = 3.0;
    pub_command.publish(driver_msg);  // publish command, but note that is possible that
                                          // the subscriber will not receive this message.
    spincount = 0;
    while (isReady) { // Waiting for isReady to be false meaning that the driver has the new command
        ros::spinOnce();  // Allow other ROS functionallity to run
        loop_rate.sleep(); // Sleep and wake up at 1/20 second (1/SPIN_RATE) interval
        if (spincount > SPIN_RATE) {  // if isReady does not get set within 1 second re-publish
            pub_command.publish(driver_msg);
            // ROS_INFO_STREAM("Just Published again driver_msg");
            spincount = 0;
        }
        spincount++;  // keep track of loop count
    }
    ROS_INFO_STREAM("waiting for rdy");  // Now wait for robot arm to reach the commanded waypoint.

    while(!isReady)
    {
        ros::spinOnce();
        loop_rate.sleep();
    }
    ROS_INFO_STREAM("Ready for new point");
}


/* set_suction
    description: this will turn the robot's suction on or off
    input: integer with value either 0 (to turn suction off) or 1 (to turn suction on)
    output: 0 if successful, 1 if unsuccessful or input invalid */
int ImageConverter::set_suction(bool suction_state)
{
    if (suction_state) {
        srv.request.fun = 1;  //Set function active
        srv.request.pin = 0;  //Digital Output 0
        srv.request.state = 1.0; //Set DO0 on
        srv_SetIO.call(srv); //Call to IO

    } else {
        srv.request.fun = 1;
        srv.request.pin = 0; // Digital Output 0
        srv.request.state = 0.0; //Set DO0 off
        srv_SetIO.call(srv); //Call to IO
    }
}
