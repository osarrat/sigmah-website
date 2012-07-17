Overview:
---------
The mailarchive module allows you to subscribe to one or more mailing lists,
messages from which will be stored in a threaded archive.  It is then possible
for users to browse and search these archives using the mailarchive_view module.


Features:
---------
 o Written in PHP specifically for Drupal.
 o Allows subscriptions to any number of mailing lists
 o Mailing list subscriptions are nodes, offering full support for taxonomy,
   comments, etc.
 o Utilizes message_id, in_reply_to, and reference headers to offer fully
   threaded archive.
 o Provides a browsable mailing list overview page similar to Drupal forums, 
   offering an at a glance look at how active each list is, who sent the last
   message, and what it was about.


Requires:
---------
 - Drupal 6.0
 - PHP configured with IMAP support (http://php.net/imap)


Credits:
--------
 - Written by Jeremy Andrews <jeremy@tag1consulting.com>
 - Maintained by Jeremy Andrews <jeremy@tag1consulting.com>


Example:
--------
 - http://kerneltrap.org/mailarchive

