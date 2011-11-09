// $Id: README.txt,v 1.4 2009/07/11 21:36:46 anrikun Exp $

MENU FIRSTCHILD

DESCRIPTION
By default, Drupal 6 requires that you enter a path for each menu link you add/edit from the Menu administration page.
There are cases you may want to create a parent item, without any path, that simply links to its first viewable child item.
Menu Firstchild provides this functionality.

INSTALLATION
Copy the menu_firstchild folder to your sites/all/modules directory.
Navigate to admin/build/modules and enable the module
Navigate to admin/build/menu and click on the menu you to customize
Add a new menu link or edit an existing link then enter <firstchild> as the link path to turn it into a parent item linking to its first viewable child.

UPDATING
Replace the older menu_firstchild folder by the newer one.
Then...

Updating from v6.0:
Visit update.php.

Updating from v6.1:
After updating, go back to your menus' "Edit Menu Items" pages (e.g. /admin/build/menu-customize/primary-links).
If there are <firstchild> menu items disabled when they should not, just enable them again and save changes. They will keep their states now.

KNOWN ISSUES
Breadcrumb might be wrong. This is not a bug of Menu Firstchild but a problem linked to Drupal's poor default breadcrumb system.

CONTACT
Henri MEDOT <henri.medot[AT]absyx[DOT]fr>
http://www.absyx.fr
