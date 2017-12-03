IF NOT EXIST "REPLACE\ece391_share\work\vm\" (mkdir "REPLACE\ece391_share\work\vm\")
IF NOT EXIST "REPLACE\ece391_share\work\vm\devel.qcow" ("REPLACE\qemu_win\qemu-img.exe" create -b "REPLACE\image\ece391.qcow" -f qcow2 "REPLACE\ece391_share\work\vm\devel.qcow") ELSE (echo devel.qcow already exists.)
IF NOT EXIST "REPLACE\ece391_share\work\vm\test.qcow" ("REPLACE\qemu_win\qemu-img.exe" create -b "REPLACE\image\ece391.qcow" -f qcow2 "REPLACE\ece391_share\work\vm\test.qcow") ELSE (echo test.qcow already exists.)
