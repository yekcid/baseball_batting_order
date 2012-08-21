========================================================================
    CONSOLE APPLICATION : baseball_batting_order Project Overview
========================================================================

AppWizard has created this baseball_batting_order application for you.

This file contains a summary of what you will find in each of the files that
make up your baseball_batting_order application.


baseball_batting_order.vcxproj
    This is the main project file for VC++ projects generated using an Application Wizard.
    It contains information about the version of Visual C++ that generated the file, and
    information about the platforms, configurations, and project features selected with the
    Application Wizard.

baseball_batting_order.vcxproj.filters
    This is the filters file for VC++ projects generated using an Application Wizard. 
    It contains information about the association between the files in your project 
    and the filters. This association is used in the IDE to show grouping of files with
    similar extensions under a specific node (for e.g. ".cpp" files are associated with the
    "Source Files" filter).

baseball_batting_order.cpp
    This is the main application source file.


The solution reads in a comma separated text file of batting stats with the following data:

Player Name,AB,BB,H,1B,2B,3B,HR,RBIs,Runs,SF,HBPK,SB,AVG,OBP,SLB,OPS
The batting order is generated as such from the data:
1. SB leader (that is not present in positions 3-5) with the highest batting AVG.
2. Player with the highest OBP (that is not present in positions 3-5)
3. Player with the higest OPS
4. Player with the 2nd higest OPS
5. Player with the 3rd higest OPS
6.-last position is filled in by the remaining players in the list with starting from highest to lowest AVG