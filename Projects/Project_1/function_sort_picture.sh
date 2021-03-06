

function sort_picture
	cd $argv[1]
	find . -name "*.jpg" | while read line;
		echo $line
		set folder (eval date -r $line +%Y/%B/%d)
		echo $folder
		if test -d $argv[2]/$folder
			cp $line $argv[2]/$folder
		else
			mkdir -p $argv[2]/$folder
			cp $line $argv[2]/$folder
		end
	end
end

sort_picture "path_to_be_sorted" "path_to_save_sorted_files"
