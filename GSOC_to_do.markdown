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
   * issue : (0000418:Improve image insertion usability)
   * issue : (0000328:Add a management of “blockquote” HTML tag)
   * issue : (0000455: Better Integration of the Issues Tracker with the Sigmah-website project)
5) Better Image integration with the WYSIWYG Editor.
   * Add support for blockquotes, and other such HTML styling features which are currently not supported
   * Work on the theming of the current form, to give it a more aesthetic and engaging feel.
   * Ability to insert title and link to issues just by typing a pattern in the wysiwyg editor (viz. say typing #_393_ would map to issue number 393 in the sigmah issue tracker and be replaced by 393:Sample Issue Title, and also automatically hyperlinked to the particular issue.
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

May 21st - May 23rd : Automatic Revision for user Guide
May 24th - May 26th : Diff between revisions of manual pages
May 27th - May 28th : Fixing and debugging any code that might break after these two are set up, and theming to go well with the current custom theme : sigmah_theme
may 29th - June 2nd : Setting up the Mailman server and configuring it for our own needs
June 3rd - June 7th : Initial Integration of the mailman server with our Drupal setup, to expose mailman parameters to Drupal.
June 8th - June 12th : Custom addition and deletion of users to mailing lists based on their Drupal user profile roles.
June 13th - June 18th : Let Drupal users access mailman archives from within Drupal sigmah website
June 19th - June 21st : Fixing and debugging any code that might break after the integration of the mailman server with Drupal, and theming the mail archives to go well with our current custom theme : sigmah_theme

June 23rd - June 29th : Integration of the Drupal WYSIWYG API for better Image integration
June 30th - July 2nd : Support for blockquotes and other styling features which are currently not supported
July 3rd - July 5th : Ability to insert title and link of issues from the Mantis database, just by typing a pattern in the WYSIWYG editor
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