 #include "sound.h"

 static void play_sound(uint32_t nFrequence)
 {
  	uint32_t Div;
  	uint8_t tmp;

     //Set the PIT to the desired frequency
  	Div = 1193180 / nFrequence;
  	outb(0xb6, 0x43);
  	outb((uint8_t) (Div), 0x42);
  	outb((uint8_t) (Div >> 8), 0x42);

         //And play the sound using the PC speaker
  	tmp = inb(0x61);
   	if (tmp != (tmp | 3))
     {
  		outb(tmp | 3, 0x61);
  	}
  }

 // G - 392
 // A# - 466.16
 // C - 523.25
 // F - 349.23
 // F# - 369.99
 static void shut_up()
 {
    uint8_t tmp = inb(0x61) & 0xFC;

    outb(tmp, 0x61);
 }

 /*
  * sound_write
  *   DESCRIPTION: A complete waste of space
  *   INPUTS: nothing we ever use
  *   OUTPUTS: nada
  *   RETURN VALUE: 0. Cuz that helps.
  *   SIDE EFFECTS: It might as well be invisible
  */
int32_t sound_open (const int8_t* filename)
{
    return 9; // twiddle our thumbs
}

/*
 * sound_write
 *   DESCRIPTION: Plays mewwwwwwsic :D
 *   INPUTS: File descriptor, a buffer of two uint32_ts: the first is
 *           the frequency, the second is how long in deciseconds to play it
 *   OUTPUTS: Sends bytes to ports responsible for the PC speaker
 *   RETURN VALUE: 0 on success, -1 on failure
 *   SIDE EFFECTS: Music, but possible noise. So watch out.
 */
int32_t sound_write (int32_t fd, const void* buf, int32_t nbytes)
{
    if(nbytes != 8)
        return -1;

    /* Play, wait, stop. */
    uint32_t* input = (uint32_t*)buf;
    play_sound(input[0]);
    sleep_deciseconds(input[1]);
    shut_up();

    return 0;
}

/*
 * sound_read
 *   DESCRIPTION: Doesn't make sense to read from sound
 *   INPUTS: Doesn't matter, we ain't using them
 *   OUTPUTS: Absolutely nothing results from this being called
 *            other than calling the caller an idiot
 *   RETURN VALUE: -1 on it's inevitable failure
 *   SIDE EFFECTS: Hurt feelings
 */
int32_t sound_read (int32_t fd, void* buf, int32_t nbytes)
{
    return -1;
}

/*
 * sound_close
 *   DESCRIPTION: why
 *   INPUTS: nope
 *   OUTPUTS: just stop
 *   RETURN VALUE: can't you fuckin' read
 *   SIDE EFFECTS: I can't even
 */
int32_t sound_close (int32_t fd)
{
    return 0;
}

void MI()
{
    uint32_t g[2] = {392, 40};
    uint32_t a_sharp[2] = {466, 30};
    uint32_t c[2] = {523, 30};
    uint32_t f[2] = {349, 30};
    uint32_t f_sharp[2] = {370, 30};

    sound_write(0, g, 8);
    sleep_deciseconds(20);
    sound_write(0, g, 8);
    sleep_deciseconds(20);

    sound_write(0, a_sharp, 8);
    sleep_deciseconds(10);
    sound_write(0, c, 8);
    sleep_deciseconds(10);

    sound_write(0, g, 8);
    sleep_deciseconds(20);
    sound_write(0, g, 8);
    sleep_deciseconds(20);

    sound_write(0, f, 8);
    sleep_deciseconds(10);
    sound_write(0, f_sharp, 8);
    sleep_deciseconds(10);

    sound_write(0, g, 8);
}
