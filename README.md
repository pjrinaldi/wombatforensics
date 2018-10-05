# wombatforensics

Wombat Forensics is a new Forensic Analysis tool built entirely in C and C++. The GUI is built using Qt5, so it will work on Windows, Linux and Macintosh systems. The application is designed to be user friendly, and always maintain GUI responsiveness. Having used AD Lab, X-Ways, Encase and Autopsy, I found various things that would bother me or were slow and unresponsive. I decided for a resource intensive application using C/C++ was important.

I didn't want to reinvent the wheel, so I am making use of the sleuthkit's library of c functions. Why is user friendly so important?

If you've ever used AD Lab, you have experienced GUI freezes all over the place which is annoying and interrupts workflow and train of thought. If you've ever used X-Ways, some of the options are very confusing and not explained very well or intuitive as to their meaning. AD Lab has too many options available to the user. For instance, I don't need to multi-select items if you have checkboxes available. I shouldn't have 3 ways to do the same thing. A program can be robust, but provide a sensible set of features/options to a user.

Wombat Forensics is user oriented in its design. You can select an item or check multiple items. The program is multithreaded. The GUI is always responsive because resource intensive tasks are done on different threads. Large files will not slow down the application. The hex viewer was designed to load portions of a file at a time, therefore it is always fast to view and does not eat up RAM.

I find it a waste of real estate to have a treeview in one frame and a table listing in another frame. Therefore I have combined the file structure treeview and the file information table into one view. The columns autoresize when data is loaded or when parents are expanded, ensuring the data can be read in the column without having to resize the columns manually.

I run timezone data in UTC for all features. I always prefer to work in UTC and then would like to have an option to automatically convert to a preferred timezone when generating the report.

Closed source tools are a black box. Wombat Forensics is open source and integrates open source libraries such as the sleuthkit and libewf, enabling an examiner to review code to find out how the tool yields results and verify that the results are what they should be.

With all that said and done, I am a single developer who has a full time job and spends about 3 hours/day for 3 days/week (avg) working on this project. Plus to ensure it is the most efficient and most user friendly interface, I have no issues reworking entire portions to fix issues.
