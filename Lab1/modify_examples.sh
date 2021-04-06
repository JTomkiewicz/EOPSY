#!/usr/local/bin/bash

waitForUser() {
    # wait until user press any key
    echo
    read -p "Press enter to proceed to next test..."
    echo

    # clear cmd
    clear
}

# first test program resistance for different options

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

# END
echo "#### #    #        ##### #### #### #### #### ####"
echo "#  # #    #        #   # #  # #    #    #    #   #"
echo "#### #    #        ##### #### #### #### #### #   #"
echo "#  # #    #        #     #  #    #    # #    #   #"
echo "#  # #### ####     #     #  # #### #### #### ####"

