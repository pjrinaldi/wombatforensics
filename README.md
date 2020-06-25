# Wombat Forensics

Wombat Forensics is a new Forensic Analysis tool built entirely in C and C++. The GUI is built using Qt5, so it may one day work on Windows, Linux and Macintosh systems. The current design is Linux specific, but if there is a need I can implement various IF's to make it work on the Windows and Mac. The application is designed to be user friendly, fast, and always maintain GUI responsiveness. Having used AD Lab, X-Ways, Encase and Autopsy, I found various things that would bother me or were slow and unresponsive. I decided for a resource intensive application, using C/C++ was important.

Current Features
-----------------
* Tagging - You can Tag (bookmark) files by simply right clicking on a file and selecting the tag to apply.
* Report Preview - This feature generates the report in a preview window as you add evidence or tag files to different sections. If thumbnailing has been done, then thumbnails will display properly in the preview report.
Publish Report - Publish a report and export/link files to the HTML Report. Places the report in the reporting directory set by the user from the settings dialog and in a unique sub folder based on date/time.
* Manual Carving - You can manually carve sections from the hex and tag them to your report.
* Image/Video Thumbnailing Viewer is a separate window, and once loaded has no lag when scrolling. You can go as fast as you want.
* If running a large ingest or just need a break, you can launch xchomp (pacman clone).
* Any post processing (digging deeper) is done in the background on other threads, so you can continue to review the evidence without any lag.
* Filtering will gray out non-matches, rather than hide the files in case you still want to look at them.
* You can have multiple "File Properties", "File Hex", "Viewer Windows", etc. open for different files for comparative analysis.
* File signature analysis is done at ingest.
* Hashing files when "Digging Deeper" (cause Wombat's dig). (MD5, SHA1, or SHA256).

Current Viewers
--
* Image
* Video
* Text
* HTML
* External Viewer feature where you can specify external programs

Comparison with Other Forensic Suites
--
* Coming Soon...

Reasoning
--

I wanted to learn improve my general forensic knowledge and understanding of what the existing tools are doing as well as learn more about specific artifacts. One way to do that is to build a forensic tool. This will give me better knowledge of how the black box tools are working. I didn't want to reinvent the wheel, so I am making use of as many open source libraries and tools as possible, to include the sleuthkit, libaff, libewf, liblnk, libmagick, and so on.

Why is user friendly so important?

When I started this project, there were no GUI forensic tools for linux, and so I would be working with the major GUI forensic tools in Windows. I don't like Windows very much, and all the existing GUI forensic tools have issues that bothered me. They would experience GUI freezes or GUI tearing and redrawing issues which is annoying and interrupts my workflow and train of thought. Some of the tools have too m any ways to do the same thing or options that are very confusing and not explained very well or intuitive as to their meaning. I shouldn't have 3 ways to do the same thing, it is overkill and poor design. A program can be robust, but provide a sensible set of features/options to a user.

Wombat Forensics is user oriented in its design. You can select an item or check multiple items. The program is multithreaded. The GUI is always responsive because resource intensive tasks are done on different threads. Large files will not slow down the application. The hex viewer was designed to load portions of a file at a time, therefore it is always fast to view and does not eat up RAM.

I find it a waste of real estate to have a treeview in one frame and a table listing in another frame. Therefore I have combined the file structure treeview and the file information table into one view. The columns autoresize when data is loaded or when parents are expanded, ensuring the data can be read in the column without having to resize the columns manually.

I run timezone data in UTC for all features. I always prefer to work in UTC and then there is an option to automatically convert to a preferred timezone when generating the report. This timezone selection is in the settings dialog. If you change the timezone, the preview report will update the dates and times accordingly, as well as the timezone listed in the report. It will not update an already generated "Published Report", you will have to generate a new report to show the new timezone.

Closed source tools are a black box. Wombat Forensics is open source and integrates open source libraries such as the sleuthkit and libewf, enabling an examiner to review code to find out how the tool yields results and verify that the results are what they should be.

With all that said and done, I am a single developer who has a full time job and spends about 3 hours/day for 3 days/week (avg) working on this project. Plus to ensure it is the most efficient and most user friendly interface, I have no issues reworking entire portions to fix issues.
