"""
html_generator.py
************************

:module: html_generator

:synopsis: This module used to generate html file with tables.

.. currentmodule:: html_generator

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


class HtmlGenerator:

    """ This class contains functions to generate html page and html table. It includes
    internal CSS which will be used for viewing the html page locally and it also 
    includes jenkins style sheets which will be used to view the html page in jenkins 
    job using HTML report publisher plugin.
    
    Pass the name of the html file to be created while creating the class object.
    """

    def __init__(self, filename=''):

        self.html_file_name = filename  # assigning name of the html file
        self.html_file = ''

    # Generated an html table with columns

    def html_create_page(self, title):
        """
        Creates a basic html page using html tags.
        
        Basic jenkins sheets style is included as external sheets 
        are not supported
        
        Internal CSS style is also added to the html page for viewing
        the html page locally.
        
        .. note::
           The internal CSS is currently not supported by jenkins, but
           it is required for viewing the html page locally or in a browser.
        
        :param title: It is the title of the html page
        :return: this function do not return anything.
        """

        self.html_file = open(self.html_file_name, "w")  # open the html file in write mode
        self.html_file.write('<!DOCTYPE html>\n<html lang="en">\n')
        self.html_file.write('<head>\n')  # start the head tag

        # Including jenkins style sheets as external stylesheets are not supported
        self.html_file.write('<link rel="stylesheet" type="text/css" href="/jenkins/static/ab5947fa/css/style.css">\n')
        self.html_file.write('<link rel="stylesheet" type="text/css" href="/jenkins/static/ab5947fa/css/color.css">\n')
        # End of inclusion

        # Add internal CSS
        self.html_file.write('<style type="text/css">\n')
        self.html_file.write('div.Top {margin:0 auto; width:100%; text-align: center; border:solid; }\n')
        self.html_file.write('div.Main {margin:0 auto; position: relative; float: left; left: 50%; }\n')
        self.html_file.write('div.Sub {float: left; margin-left: 20px; margin-right: 20px; position'
                             ':relative; left: -50%; }\n')
        self.html_file.write('table {border-color: gray; border-collapse: collapse; font-size: 14px; }\n')
        self.html_file.write('div.Pad {float: left; position: relative; left: -50%; }\n')
        self.html_file.write('td {padding-left: 5px; padding-right: 5px; }\n')
        self.html_file.write('#Col1 {text-align: left; }\n')
        self.html_file.write('#Col2 {text-align: left; }\n')
        self.html_file.write('#Col3 {text-align: left; }\n')
        self.html_file.write('</style>\n')
        # End of Internal CSS

        self.html_file.write('</head>\n')  # close the head tag
        self.html_file.write('<body>\n')  # close the body tag

        # Add title to the page
        self.html_file.write('<div class="Top">\n' + '<font size="6">' + title + '</font></div>\n')

        self.html_file.close()  # close the html file

    def html_add_hyperlink(self, text, link):
        """
        Add hyper link to the html page
        :param text: This text will be visible on the html page
        :param link: it contains the url.
        :return:  None
        """

        self.html_file = open(self.html_file_name, "a")  # open the html file in append mode

        # Add link to the page
        self.html_file.write('<p><font size="4"><a href=' + link + '>' + text + '</a></p>\n')

        self.html_file.close()  # close the html file

    def html_create_table(self, table_title, col1_title, col2_title, col3_title):
        """
        Creates the table in html page with 3 columns.
        
        :param table_title: Title of the table to be created
        :param col1_title: Title of column one of the table
        :param col2_title: Title of column two of the table
        :param col3_title: Title of column three of the table 
        :return: None
        """

        self.html_file = open(self.html_file_name, "a")  # open the html file in append mode

        self.html_file.write('<div class="Main tabBar">\n')
        self.html_file.write('<div class="bssPad setting-leftspace tab"><br></div>\n')
        self.html_file.write('<div class="Sub tab">\n')
        self.html_file.write('<h3>' + table_title + '</h3>\n')
        self.html_file.write('<table border="1" class="pane">\n')
        self.html_file.write('<tr bgcolor="#d5cac8">')
        self.html_file.write('<th>' + col1_title + '</th>')
        self.html_file.write('<th>' + col2_title + '</th>')
        self.html_file.write('<th>' + col3_title + '</th>')
        self.html_file.write('</tr>\n')

        self.html_file.close()  # close the html file

    def html_append_table(self, data_in_list):
        """
        Add elements to the table which is created by 
        the above function "html_create_table". 
        
        :param data_in_list: Contains the data in a list format. The list should
                             should only contain three string values exactly which
                             will be written to the three columns correspondingly.
        :return: None 
        """
        self.html_file = open(self.html_file_name, "a")

        self.html_file.write('<tr>')
        self.html_file.write('<td id="Col1">' + str(data_in_list[0]) + '</td>')
        self.html_file.write('<td id="Col2">' + str(data_in_list[1]) + '</td>')
        self.html_file.write('<td id="Col3">' + str(data_in_list[2]) + '</td>')
        self.html_file.write('</tr>\n')

        self.html_file.close()  # close the html file

    def html_close_table(self):
        """
        Closes the table created
        :return: None
        """
        self.html_file = open(self.html_file_name, "a")

        self.html_file.write('</tr>\n')
        self.html_file.write('</table>\n')
        self.html_file.write('</div>\n')

        self.html_file.close()  # close the html file

    def html_end_page(self):
        """
        Writes the closing tags to the html file.
        :return: None
        """
        self.html_file = open(self.html_file_name, "a")
        self.html_file.write('</div>\n')
        self.html_file.write('</body>\n</html>\n')

        self.html_file.close()  # close the html file
