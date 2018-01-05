A fish shell script containing a function. This function is called with two command line arguments and finds all 
the "*.jpg" files which are inside the folder path described by the first argument. For each file that is found, 
we find the modification date by the `eval date` command and `set` a folder variable with the result of the
`eval date` command using the format Y/M/D. We then check if inside the path given as a second argument already exist
a path equal to the result of the `eval date` command. If not we create one and copy the ".jpg" file there. If it 
exists we just copy the ".jpg" file there. For example: if there is a .jpg file with modification date 26 September 2016,
this file is copied to this path: `$argv[1]/$argv[2]/2016/September/26`. 

Feel free to send me any comment!
