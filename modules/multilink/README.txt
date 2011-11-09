$Id:

Multilink module for Drupal 6.x
===============================

Description:

Three modules:
 * An "input filter" module which allows links to nodes to be generated automatically to point to the correct language version of the node.
 * A redirector that behaves in a similar way to the Global Redirect module but provides specific features for multi-language, e.g. to reirect to a suitable translation if available.
 * An integration module for the Secure Pages module which allows redirection and links on node pages to be point to translations if applicable, whilst maintaining SP functionality.
 
MultiLink Redirect has been tested in combination with Global Redirect and the two seem to co-exist successfully (though some requests may be redirected more than once.) 

Further information: http://drupal.org/project/multilink

Optional patch to filter.module
===============================
Optionallly install the included patch check_markup_language_patch_1.patch - this allows filter output to be cached by Drupal's filter caching mechanism on a per-language basis, so potentially increasing performance considerably.  It will not be necessary in Drupal 7 as that already includes the functionality provided by this patch. The patch needs to be applied to filter.module in the drupal/modules directory.

Contact: http://www.netgenius.co.uk/contact

