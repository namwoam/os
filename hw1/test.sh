echo "Output for problem 3.14:"
./3_14 25
echo "Output for problem 3.15:"
./3_15 25
echo "Test for problem 3.20:"

old_file="./bible.txt"
new_file="./new_bible.txt"
./filecopy "$old_file" "$new_file"

if cmp -s "$old_file" "$new_file";
then
    echo "Test passed."
else
    echo "Test failed."
fi