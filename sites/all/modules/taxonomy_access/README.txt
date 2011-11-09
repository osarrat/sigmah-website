-----------------------
GENERAL DESCRIPTION
-----------------------
This module allows you to set access permissions for various taxonomy 
categories based on user role.  

There are permissions to VIEW, UPDATE, 
and DELETE nodes in each category, as well as permissions for whether a 
user can CREATE new nodes in a certain category or see the category LISTED.


-----------------------
HELP PAGES
-----------------------
For more information about how to control access permissions with the Taxonomy
Access Control module, see the module's help page at:
"Administer >> Help >> Taxonomy Access Control"
(Drupal path: admin/help/taxonomy_access).

Also see the help pages at drupal.org: http://drupal.org/node/31601


-----------------------
DATABASE TABLES
-----------------------
Module creates two tables in database: 'term_access' and
'term_access_defaults'


-----------------------
TROUBLESHOOTING:
-----------------------

If users can view or edit pages that they do not have permission for:

1. Make sure the user role does not have "administer nodes" permission.  This 
   permission will override any settings from Taxonomy Access.

2. Check whether the user role has "edit any [type] content" permissions 
   under "node module" on the page: 
   "Administer >> User management >> Permissions"
   (Drupal path: admin/user/permissions).

   Granting this permission overrides TAC's "Update" permissions for the given 
   content type, so you will not be able to deny the role edit access to any 
   nodes in that type.  (The same is true of "delete any [type] content" 
   permissions.)

3. Check to see if the user has other roles that may be granting other 
   permissions. Remember: Deny overrides Allow within a role, but Allow from 
   one role can override Deny from another.

4. Review the configuration for the authenticated user role on page:
   "Administer >> User Managemet >> Taxonomy Access Permissions"
   (Drupal path: admin/user/taxonomy_access/edit/2).

   Remember that all custom roles also have the authenticated role, so 
   they gain any permissions granted that role.

5. Check whether you have ANY OTHER node access modules installed.
   IMPORTANT: When using multiple access control modules, permissions are 
   ALWAYS OR-ed together.  If one module grants permissions for a given 
   user or role for a node, then user CAN view/edit/delete even if another 
   module DENIES it.

6. Do a General Database Housekeeping
  (Tables: 'node_access','term_access' and 'term_access_defaults'):

  First DISABLE, then RE-ENABLE the Taxonomy Access module on page:
  "Administer >> Site building >> Modules".
  (Drupal path: admin/build/modules).
    
  This will force the complete rebuild of the 'node_access' table.
  
7. For debugging, please install deve_node_access module (Devel module)
   This can show you some information about node_access values in 
   the database when viewing a node page.

8. Force rebuilding of the permissions cache (table 'node_access'):
   "Rebuild permissions" button on page: 
   "Administer >> Content Management >> Post settings"
   (Drupal path: admin/content/post-settings/rebuild)

   If the site is experiencing problems with permissions to content, you may
   have to rebuild the permissions cache. Possible causes for permission
   problems are disabling modules or configuration changes to permissions.
   Rebuilding will remove all privileges to posts, and replace them with
   permissions based on the current modules and settings.

-----------------------
UNINSTALL
-----------------------

1. After disabling module, you can uninstall completely by choosing Taxonomy
   Access on page: 
   "Administer >> Site building >> Modules >> Uninstall"
   (Drupal path: admin/build/modules/uninstall).

   This will remove all your settings of Taxonomy Access: variables and tables
   ('term_access' and 'term_access_defaults').

2. After uninstall, if the site is experiencing problems with permissions to
   content, you can rebuild the permission cache.
   See "Troubleshooting" #8.

-----------------------
CONTACT
-----------------------

Please send bug reports, feature requests, or other comments about Taxonomy
Access Control module: http://drupal.org/project/issues/taxonomy_access

You can also contact me personally:
Keve  ( http://drupal.org/user/13163/contact )
