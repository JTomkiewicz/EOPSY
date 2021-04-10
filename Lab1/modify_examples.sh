#!/usr/local/bin/bash

# function that waits for user with next test
waitForUser() {
    # wait until user press any key
    echo
    read -p "Press enter to proceed to next test..."
    echo

    # clear cmd
    clear
}

# first test program resistance for different options

clear

# TEST CASE 1
echo "TEST CASE 1: Print help using -h"
echo
./modify -h
waitForUser

# TEST CASE 2
echo "TEST CASE 2: Run modify with not defined option"
echo
./modify -k
waitForUser

# TEST CASE 3
echo "TEST CASE 3: Run modify without any options"
echo
./modify
waitForUser

# TEST CASE 4
echo "TEST CASE 4: Run modify with both -u and -l"
echo
./modify -u -l
waitForUser

# TEST CASE 5
echo "TEST CASE 5: Run modify with without sed pattern inserted"
echo
./modify "test.txt"
waitForUser

# TEST CASE 6
echo "TEST CASE 6: Run modify with recursion and without sed pattern inserted"
echo
./modify -r "test.txt"
waitForUser

# TEST CASE 7
echo "TEST CASE 7: Run modify on not existing file"
echo
./modify -u "test.txt"
waitForUser

# second test program on files

# create some examplary data
mkdir folderForTests
cd folderForTests
touch "text.txt"
mkdir folderForTests_1
mkdir folderForTests_2
cd folderForTests_1
touch "text?#%.txt"
touch "text!.txt"
cd ..
cd folderForTests_2
touch "text2.txt"
touch "text_2_2.txt"
cd ..
cd ..

# TEST CASE 8
echo "TEST CASE 8: Upperize letters in text.txt"
echo "Before modify:"
find . -mindepth 2 -maxdepth 2 -type f
./modify -u "folderForTests/text.txt"
echo
echo "After modify:"
find . -mindepth 2 -maxdepth 2 -type f
waitForUser

# TEST CASE 9
echo "TEST CASE 9: Lowerize letters in TEXT.txt"
echo "Before modify:"
find . -mindepth 2 -maxdepth 2 -type f
./modify -l "folderForTests/TEXT.TXT"
echo
echo "After modify:"
find . -mindepth 2 -maxdepth 2 -type f
waitForUser

# TEST CASE 10
echo "TEST CASE 10: Change text.txt with sed pattern s/ext/oster/"
echo "Before modify: "
find . -mindepth 2 -maxdepth 2 -type f
./modify "s/ext/oster/" "folderForTests/text.txt"
echo
echo "After modify: "
find . -mindepth 2 -maxdepth 2 -type f
waitForUser

# TEST CASE 11
echo "TEST CASE 11: Upperize files names using recursion"
echo "Before modify: "
find . -mindepth 2 -type f
./modify -r -u "folderForTests"
echo
echo "After modify: "
find . -mindepth 2 -type f
waitForUser

# TEST CASE 12
echo "TEST CASE 12: Lowerize files names using recursion"
echo "Before modify: "
find . -mindepth 2 -type f
./modify -r -l "FOLDERFORTESTS"
echo
echo "After modify: "
find . -mindepth 2 -type f
waitForUser

# TEST CASE 13
echo "TEST CASE 13: Change files names with sed pattern s/ext/est/ and recursion"
echo "Before modify: "
find . -mindepth 2 -type f
./modify -r "s/ext/est/" "folderfortests"
echo
echo "After modify: "
find . -mindepth 2 -type f
waitForUser

# remove folder with test data
rm -r folderfortests

# END
echo "#### #    #        ##### #### #### #### #### ####"
echo "#  # #    #        #   # #  # #    #    #    #   #"
echo "#### #    #        ##### #### #### #### #### #   #"
echo "#  # #    #        #     #  #    #    # #    #   #"
echo "#  # #### ####     #     #  # #### #### #### ####"

