"""
memory_analyser.py
************************

:module: memory_analyser

:synopsis: This script is used for parsing and extracting the size information for software components from a .a file.

.. currentmodule:: memory_analyser

.. moduleauthor:: rce5cob

:date: 25 May 2018

:Version: v0.0.1

:Release: 

:Copyright: (C) Bosch Connected Devices and Solutions GmbH.
    All Rights Reserved. Confidential.
    Distribution only to people who need to know this information in
    order to do their job.(Need-to-know principle).
    Distribution to persons outside the company, only if these persons
    signed a non-disclosure agreement.
    Electronic transmission, e.g. via electronic mail, must be made in
    encrypted form.

"""


import sys
import re
from subprocess import check_output
import shutil
import argparse

from config import *
from html_generator import HtmlGenerator


def find_dot_a_file(file_path):
    """
    This function find the .a file in a directory.
    It can only one .a file and the moment a .a file
    is found it will return that filename
    :param file_path: contains the path to the directory of .a file 
    :return: returns the name of the .a file found in the directory
    """
    for file in os.listdir(file_path):
        if '.a' in file:
            return file


def get_dot_o_file_list(contents_of_archive):
    """
    This function parse the meta data of an archived folder or .a file
    which is passes to this function as parameter. It parse all the .o files
    and store them in a list and finally this list of .o files
    is returned.
    :param contents_of_archive: contains the metadata of the .a file or details of files inside it
    :return: returns the .o file list
    """

    dot_o_file_list = []  # list to store the .o files.

    # find all the lines with .o files using regular expressions.
    extract_data = re.findall(
        r'\d{4}-\d{2}-\d{2} \d{2}:\d{2}:\d{2}\s{1,5}.....\D{1,20}\d{1,10}\D{1,20}\d{1,10}\s{1,5}\w{1,50}.o',
        contents_of_archive)

    # get .o file from extract_data.
    for data in extract_data:
        # convert the data to a list
        datatolist = (" ".join(re.split("\s+", str(data), flags=re.UNICODE))).split(" ")
        dot_o_file_list.append(datatolist[5])  # append the .o file

    return dot_o_file_list


def get_dot_txt_file_list(contents_of_archive):
    """
    This function parse the meta data of an archived folder or .a file which
    is passed to this function as parameter.
    :param contents_of_archive: contains the metadata of the .a file or details of files inside it
    :return: returns the list of .txt file and it's size
    """
    dot_txt_file_list = []  # list to store the .txt files and it's corresponding sizes.

    # find all the lines with .txt files using regular expressions.
    extract_data = re.findall(
        r'\d{4}-\d{2}-\d{2} \d{2}:\d{2}:\d{2}\s{1,5}.....\D{1,20}\d{1,10}\D{1,20}\d{1,10}\s{1,5}\w{1,50}.txt',
        contents_of_archive)

    # get .txt file from extract_data.
    for data in extract_data:
        # convert the data to a list
        data_to_list = (" ".join(re.split("\s+", str(data), flags=re.UNICODE))).split(" ")
        dot_txt_file_list.append(data_to_list[5])  # append the .txt file name
        dot_txt_file_list.append(data_to_list[3])  # append the size of the .txt file

    return dot_txt_file_list


def main(argv):

    # assign the arguments
    parser = argparse.ArgumentParser()
    parser.add_argument("output_file", help="Enter the name of the output html file")
    parser.add_argument("input_level", help="Enter the level of the report to be generated")

    args = parser.parse_args()

    # assigning the input level to argument to a variable
    input_level = int(args.input_level)

    if input_level > maxDetailLevel or input_level < 1:
        print("Detail Level", input_level, "is not supported. Setting to Level 1.\n")
        input_level = 1

    # Find the .a file in the path give and assign the file name to a variable
    dot_a_file_name = find_dot_a_file(dotAFilePath)

    # .a file with full path
    file_to_extract = dotAFilePath + '\\' + dot_a_file_name

    target_folder = file_to_extract[:-2]  # folder to extract the .a file

    # Delete the old target folder if exist
    try:
        shutil.rmtree(target_folder)
    except:
        pass

    # Remove memory report directory if exist, to delete old report
    if os.path.isdir(reportDirectory):
        shutil.rmtree(reportDirectory)

	# Now that the old directory was removed, creating new one
    os.makedirs(reportDirectory)

    # append the path to the output file
    outfile = reportDirectory + '/' + args.output_file

    # Extract the zip folder
    try:
        check_output([sevenZipExePath, 'x', file_to_extract, '-o'+target_folder])
    except:
        print('Unable to extract the .a file using 7zip tool')
        sys.exit(1)

    # Initialized local variables

    html_generator = HtmlGenerator(outfile)
    module_wise_data_list = []
    module_wise_data = dict(ModuleName=[], Flash=[], Ram=[], Other=[])
    total_module_size = dict(TotalFlash=0, TotalRam=0, TotalOther=0)

    # Get the list of .o files
    contents_of_archive = (check_output([sevenZipExePath, 'l', file_to_extract])).decode('utf-8')
    dot_o_file_list = get_dot_o_file_list(contents_of_archive)
    dot_txt_file_list = get_dot_txt_file_list(contents_of_archive)

    # Loop over each .o file to extract the data and save file name, FLASH size , RAM size and other
    # size in a dictionary.
    for dot_o_file in dot_o_file_list:

        module_wise_data['ModuleName'].append(dot_o_file)  # append the .o file to the list

        # extract .o file details using 7zip tool
        dot_o_file_metadata = (check_output([sevenZipExePath, 'l', target_folder + '/' + dot_o_file])).decode('utf-8')
        dot_o_file_metadata = re.findall(r'\s{1,20}.....\s{1,25}\d{1,5}\s{1,25}\d{1,5}\s{1,5}.\S{1,50}',
                                         dot_o_file_metadata)
        
        flashsize = 0  # temporary variable to calculate flash section size
        ramsize = 0  # temporary variable to calculate ram section size
        othersize = 0  # temporary variable to calculate other section size

        # calculate the RAM,FLASH and other size module wise
        for data in dot_o_file_metadata:
            # convert the data to a list
            data_to_list = (" ".join(re.split("\s+", str(data), flags=re.UNICODE))).split(" ")
            if len(data_to_list) == 5:
                # if condition for calculation the other section size.
                if firmwareOtherSectionList[0] in data_to_list[4]\
                        or firmwareOtherSectionList[1] in data_to_list[4]\
                        or firmwareOtherSectionList[2] in data_to_list[4]\
                        or firmwareOtherSectionList[3] in data_to_list[4]\
                        or firmwareOtherSectionList[4] in data_to_list[4]\
                        or firmwareOtherSectionList[5] in data_to_list[4]\
                        or firmwareOtherSectionList[6] in data_to_list[4]\
                        or firmwareOtherSectionList[7] in data_to_list[4]:
                    othersize = othersize + int(data_to_list[2])
                else:
                    # if condition for calculation the flash section size.
                    if firmwareFLASHSectionList[0] in data_to_list[4]\
                            or firmwareFLASHSectionList[1] in data_to_list[4]\
                            or firmwareFLASHSectionList[2] in data_to_list[4]:
                        flashsize = flashsize + int(data_to_list[2])

                    # if condition for calculation the ram section size.
                    if firmwareRAMSectionList[0] in data_to_list[4]\
                            or firmwareRAMSectionList[1] in data_to_list[4]\
                            or firmwareRAMSectionList[2] in data_to_list[4]:
                        ramsize = ramsize + int(data_to_list[2])

        module_wise_data['Flash'].append(flashsize)  # append the flash size to the list
        module_wise_data['Ram'].append(ramsize)  # append the ram size to the list
        module_wise_data['Other'].append(othersize)  # append the others to the list

    # Generate html file and write the data
    html_generator.html_create_page(pageTitle)

    # Add link to the html page
    html_generator.html_add_hyperlink('Memory Profiling Design',
                                      'https://bcds02.de.bosch.com/confluence/display/CDDK/Memory+Profiling')

    # calculate the total FLASH,RAM and Other size for input level 1
    if input_level == 1:

        for value in module_wise_data['Flash']:  # for loop for calculating total FLASH
            total_module_size['TotalFlash'] = total_module_size['TotalFlash'] + int(value)

        for value in module_wise_data['Ram']:  # for loop for calculating total RAM
            total_module_size['TotalRam'] = total_module_size['TotalRam'] + int(value)

        for value in module_wise_data['Other']:  # for loop for calculating total Other section
            total_module_size['TotalOther'] = total_module_size['TotalOther'] + int(value)

        # Start creating table in html report and table columns with heading Package, RAM and FLASH
        html_generator.html_create_table(tableHeadingLevelOne, 'Package', 'FLASH', 'RAM')

        # Add .txt file size to total other section
        total_module_size['TotalOther'] = total_module_size['TotalOther'] + int(dot_txt_file_list[1])

        # Append data to the html table created above
        html_generator.html_append_table([dot_a_file_name, total_module_size['TotalFlash'],
                                          total_module_size['TotalRam']])

        # Display the data in table format in console output
        print(tableHeadingLevelOne)
        print("{:<25} {:<15} {:<15}".format('Package', 'FLASH', 'RAM'))
        print("{:<25} {:<15} {:<15}".format(dot_a_file_name,
                                            total_module_size['TotalFlash'],
                                            total_module_size['TotalRam']))

    # calculate module wise total FLASH,RAM and Other size for input levle 2
    if input_level == 2:
        # Convert data in dictionary to list
        for x in range(len(module_wise_data['ModuleName'])):
            module_wise_data_list.append([module_wise_data['ModuleName'][x],
                                          module_wise_data['Flash'][x],
                                          module_wise_data['Ram'][x],
                                          module_wise_data['Other'][x]])

        # Sort the list taking module name as reference in alphabetic order
        module_wise_data_list = sorted(module_wise_data_list, reverse=False, key=lambda x: (x[0]))

        # Display the data in table format in console output
        print(tableHeadingLevelTwo)
        print("{:<50} {:<15} {:<15}".format('ModuleName', 'FLASH', 'RAM'))
        for i in range(len(module_wise_data_list)):
            print("{:<50} {:<15} {:<15}".format(module_wise_data_list[i][0],
                                                module_wise_data_list[i][1],
                                                module_wise_data_list[i][2]))

        # Start creating table in html report and table columns with heading Package, RAM and FLASH
        html_generator.html_create_table(tableHeadingLevelTwo, 'Module', 'FLASH', 'RAM')

        # Display the data in table format in console output
        for i in range(len(module_wise_data_list)):
            html_generator.html_append_table([module_wise_data_list[i][0],
                                              module_wise_data_list[i][1],
                                              module_wise_data_list[i][2]])

    html_generator.html_close_table()  # close the table by writing closing table tag
    html_generator.html_end_page()  # close the html file with the html closing tag

    shutil.rmtree(target_folder)  # Delete the extracted zip file

if __name__ == "__main__":
    main(sys.argv[1:])
