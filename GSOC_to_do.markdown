####
Proposal : 

1) ~~Automatic Revision for User Guide~~
   * ~~issue : (0000310: No automatic revision on modification of a user guide)~~
   * The current workflow requires, a user to manually add revisions when he is editing, proposed is an automatic versioning system whenever any content is edited.
2) ~~Diff between revisions of manual pages~~
   * ~~issue : (0000311:Showing diff of revisions)~~
   * The current revision system, allows any user to have revisions of a document, but there is no system by which changes made between two particular revisions could be seen. Proposed is the implementation of such a feature in sigmah.org.
3) Setting up a mailing list server, and its integration with the sigmah-website
   * issue : (0000373:Add discussion list management for Steering Cooperative mailing list)
   * This would not only allow mailing lists and private archives for the Steering Cooperative Committee, but also for any group/role present on the website (viz. Developers, Beta testers, Editors, and individual organic groups created by the users)
4) Improvements to the usability of the current forum :
   * ~~issue : (0000418:Improve image insertion usability)~~
   * ~~issue : (0000328:Add a management of “blockquote” HTML tag)~~
   * ~~issue : (0000455: Better Integration of the Issues Tracker with the Sigmah-website project)~~
5) Better Image integration with the WYSIWYG Editor.
   * ~~Add support for blockquotes, and other such HTML styling features which are currently not supported~~
   * Work on the theming of the current form, to give it a more aesthetic and engaging feel.
   * ~~Ability to insert title and link to issues just by typing a pattern in the wysiwyg editor (viz. say typing #_393_ would map to issue number 393 in the sigmah issue tracker and be replaced by 393:Sample Issue Title, and also automatically hyperlinked to the particular issue.~~
   
	 (Also Added a CKE Editor toolbar button to directly search the Sigmah Issue Tracker through AJAX Requests and insert the respective Link)
	 
6) Improvements to the current Subscriptions system :
   * issue : (0000452:Notification/Subscriptions too complex for end user)
   * Implementation of the ability to follow Forum posts even without commenting in them
7) User Dashboard :
   * Set up dashboards for user, to let them keep track of things like, Topics they have posted in, Topics they are following, and other widgets like “Most popular Forum topics”, “latest posts by a particular user” , etc.
8) User profile pictures :
   * issue : (0000322::Add a table of all community member)
   * Setting up the User Profile pictures, and then using them to build Community Galleries.
9) Deployment of all the solutions to the issues  already posted on the sigmah-issue tracker.
10) Work on the Technical Documentation of the sigmah-website project, to help other contributors understand the project better and faster.:
   * ~~issue : (0000445:Getting the sigmah website code up and working on your machine)~~



####Work Plan 


~~May 21st - May 23rd : Automatic Revision for user Guide~~
~~May 24th - May 26th : Diff between revisions of manual pages~~
~~May 27th - May 28th : Fixing and debugging any code that might break after these two are set up, and theming to go well with the current custom theme : sigmah\_theme~~
may 29th - June 2nd : Setting up the Mailman server and configuring it for our own needs
June 3rd - June 7th : Initial Integration of the mailman server with our Drupal setup, to expose mailman parameters to Drupal.
June 8th - June 12th : Custom addition and deletion of users to mailing lists based on their Drupal user profile roles.
June 13th - June 18th : Let Drupal users access mailman archives from within Drupal sigmah website
June 19th - June 21st : Fixing and debugging any code that might break after the integration of the mailman server with Drupal, and theming the mail archives to go well with our current custom theme : sigmah\_theme

~~June 23rd - June 29th : Integration of the Drupal WYSIWYG API for better Image integration~~
~~June 30th - July 2nd : Support for blockquotes and other styling features which are currently not supported~~
~~July 3rd - July 5th : Ability to insert title and link of issues from the Mantis database, just by typing a pattern in the WYSIWYG editor~~
July 6th - July 12th : Improvements to the current Subscriptions system : Ability to Follow posts not related to the user, proper theming of the new features, etc.
July 13th - July 16th : User Dashboard
July 17th - July 19th : User profile pictures, and gallery of community members
July 20th - July 27th : Deployment of all the solutions to the issues already posted on the mantis issue tracker, and not yet deployed.
July 28th - End : Work on the technical Documentation of the sigmah-website project, to help other contributors understand the project. Also deal with any other unforseen issues which might crop up.


####Self NOTES


1) ####Automatic Revision for User Guide :
   * Revision configured only for Book Page Content Type
   * The default "create revision" checkbox on the Node edit page has been removed, because it is redundant if we want to force revisions with every edit.
   * The "commit" message or revision log has not been enforced though, it depends on the user if he/she wishes to leave a commit message for the admins.
   * Expected Behaviour tested on local copy of the software !! Working Fine !!

2) ####Diff between revisions of manual pages
   * Now anyone with "view revision" permissions will see a new column for diff on the revisions page
   * The new column will be named "show diff" and will be sub divided into two more columns !! There are Radio buttons for every revision available in both the sub columns.
   * Selecting two different revisions using the two columns, and then clicking on "show diff" will show the git style diff between both the revisions with "-" signifying deletion of line and "+" signifying addition of line.
   * The diff engine is based on a GPLed phpWiki diff engine
   * Expected Behaviour tested on local copy of the software !! Working Fine !!

3) ####Ability to insert title and link to issues just by typing a pattern in the wysiwyg editor (viz. say typing #_393_ would map to issue number 393 in the sigmah issue tracker and be replaced by 393:Sample Issue Title, and also automatically hyperlinked to the particular issue.
   * the text entered through the "FUll HTML input format" and "Filtered HTML input format" are searched for regular expressions of the form  #{numbers} and they are mapped to the appropriate issue number on the sigmah issue tracker
   * I have done simple URL mapping as of now, meaning, it is NOT checked if the issue exists or not
   * Planning to include a MySQL query which looks for the particular issue in the Mantis Database and returns error if the issue doesnot exist, gives back other information like title of the issue to use in the hyperlink in the text.
   * Expected Behaviour tested on local copy of the software !! Working Fine !!
   * Planning a few modifications after getting in touch with Olivier
   * @Olivier : When can you give me a sql dump of the current Mantis Issue tracker database (Data obscured, ofcourse) , would help testing this feature.
'  * @Olivier : to refer to issues, what convention exactly to use ? the most obvious would be of the form #<numbers> , but hash can naturally occur next to a number, again creating confusion, I have used the Ruby on Rails convention for inline variables, i.e. #{numbers} , but I am not sure, how comfertable would be people using it. Let me know if you have any paricular pattern in mind.

4) ####Added a script to solve the issue number 303. Now whatever text to enter in the box before clicking on "Send" automatically appears within the appropriate text area in the Lightbox iframe that crops up. 
  * Noticed the iframe is rendered by page-contact.tpl template file, and need to confirm, is there any other way the user of the website can directly access the contact form (instead of the Lightbox iframe as in the first page)
  * Expected behaviour tested on local copy of the software !! Working Fine !!
  
 5) ####Added a hook_menu or a URI access point to get JSON data about mantis issues.
* URIs of the form 
<base-address-of-site>/issue_tracker/%/[true/false] 
would return the JSON data.
The true/false is with respect to, if we want multiple matches / suggestions or not.

So, if we have the last true/false value as false, then it will return info about only one issue.

So, 
    <base-address-of-site>/issue_tracker/12/false
	will return JSON data for issue number 12, and the data will be of the form
	{
		'issueNumber' : "12",
		'issueSummary' : "Sample SUmmary for the issue"
	}

and if there are no matches, then a JSON of the type:
{ "issueNumber": "NaN", "issueSummary": "NaN" }
is returned.


If, we want multiple results, or suggestions, then two cases will be there,
the % part of the URI, may hold a number or a string.
If it holds a number, then all issues numbers (upto a limit of 10 issues) which match the number sequence will be returned, and if the % part has a string, then all issue Summaries which have the particular sequence of string somewhere in theme will be returned.

The returned JSON data will be of the form :
[ { "issueNumber": "1", "issueSummary": "Test Summary :" }, { "issueNumber": "2", "issueSummary": "my sample summary" }, { "issueNumber": "3", "issueSummary": "my test summary regarding a new issue" } ]

These can be further utilised in any Client Side Utilites to create issue AutoComplete buttons, or issue suggestion buttons in the WYSIWYG editors, and also in the Integration of GitHub messages with the issue tracker.
And all these access points are available to anyone with access content" permission

@Olivier : If we can deploy this code on the live website, then I can work with the real Mantis issue tracker data when working on the GitHub integration and on the button on the CKE editor.

* The Database Settings have to be updated in the sigmah.module file inside sites/all/modules/sigmah/ folder , inside the funciton named connectToMantisDatabase() .

6) ####Added a CKE editor plugin to have a custom SIgmah Button in the CKE editor toolbar to insert links to Issues on the Sigmah Issue Tracker
  * I have included Auto Suggestion Feature, which would start suggesting issues as you type, similar to the feature we have in the Google Search , and we can also select multiple Issues at a time similar to the Multiple Friends Selection Feature we have on Facebook.
  * The Auto Suggestion Feature talks with the Issue Tracker Database with AJAX calls to the URI access point set up in drupal in Note Number 5.
  * User is provided with options to Configure the Link as it would be inserted in terms of, if the user wants to insert the Issue Summary along with the Issue Summary or not, and also if he wants the link to open in a new window or not.
  * I have NOT USED the default Autocomplete.js file drupal comes with, because that needed us to change the JSON format defination as done in point number 5 , and also it did not give as much flexibility as we needed for this particular task !
  * The location of the SIgmah Issue Tracker has been hard coded in a variable named linkURL  in the plugin.js file, which can be found in the sites/all/modules/ckeditor/ckeditor/plugins/IssueTracker folder. Assuming this will be a constant over time, that is why not creating any overhead form elements to take the input from the GUI interface.
  * I have used the sigmah logo for the button image in the CKE Editor toolbar, and it can be changed by using any other 16x16 img and replacing the sites/all/modules/ckeditor/ckeditor/plugins/IssueTracker/images/IssueTracker.png file with it.
  * The location of the button in the CKE Editor toolar can be determined bu using the config.toolbar\_DrupalFiltered , config.toolbar\_DrupalBasic , config.toolbar\_DrupalFull arrays, in the sites/all/modules/ckeditor/ckeditor.config.js  file.
  * Expected Behaviour tested on local copy of the software !! Working Fine !!
  * Note : This is my first CKE Editor Plugin, so It has to be tested properly before being deployed on the LIVE website, and also, it doesnot have any database updates, so the revisioning should be easier :).

7) Added an Direct Image/File Browse and Insert button in the CKE toolbar, as discussed in issue  #418, to directly browse the server and insert a file, or upload a file and then insert.
  * The Image Auto Resize will be added, after I get the required resize dimensions from Olivier. 
  * The Copy Paste Images feature as requested in the issue, seems to be a bit tough if not impossible, I will update about that, after I do my bit of research on that. As of now, postponing the feature towards the end of the programme, when we will have some extra time.
  * nevertheless, the basic  problem which needed to be addressed in the issue, seems to be solved.
  * A icon for the File Insert button on the CKE editor is now the default logo of the IMCE browser, that can be changed by changing the file : sites/all/modules/ckeditor/plugins/imce/images/icon.gif  to any othe image of dimensions (16x16px)
  * The button is located just next to the deafult button for Image insertion on CKE toolbar.
  * Expected Behaviour tested on local copy of the software !! Working Fine !!

7) Added SEO friendly URLS for all pages
  * The new content created will have an automatic alias setup
  * I have manually updated the alias of all the old content
  * the old links of the form node/123 will still work, because we are only setting up an alias
  * an user can override the automatic alias by going to "URL path settings" in the content creation page, and unchecking the checkbox saying "Automatic alias" and then fill in the desired alias in the box below
  
  
8) Added the Community Page of all Users
  * Enabled picture support, and added a default picture for all user profiles
  * User picture has a max size of 85 x 85 pixels
  * Using the User_Stats module to obtain the user statistic of post count (this can be further extended in the future with whatever user stats deemed necessary, like days registered, login count, days since last login, etc)
