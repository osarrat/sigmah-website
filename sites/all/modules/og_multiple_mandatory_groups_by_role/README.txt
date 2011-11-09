/**
 * $Id: README.txt,v 1.2.2.2 2008/07/19 16:45:30 rconstantine Exp $
 * @package OG_MMGBR
 * @category NeighborForge
 */

--Organic Groups Multiple Mandatory Groups by Role Module--

Long name, isn't it?
*Table of Contents*
 -Objective
 -Compatibility
 -Installation
 -Functionality
 -Cooperation
 -Wrap up
 -Created by

*Objective*
  The goal of this module was to extend the capabilities of the original
  og_mandatory_group module to allow as many mandatory groups as the user wants for:
  1) All users
  2) Group administrators/owners
  3) any role
  As all of the changes constitute a module with drastically different capabilities,
  it didn't make sense to patch the original module with this code. Additionally, 
  most of the old code was thrown out and a ton of new code put in place.

  Not changing Drupal core is always a design goal and was accomplished.

*Compatibility*
  This module is compatible with Drupal 6.x. Plays well with my Accounttypes module.
  
  You should probably uninstall og_mandatory_group, though it may work. I didn't
  test that setup.

*Installation*
  Installation occurs in the normal Drupal way. You shouldn't have to do anything
  extra.

  At installation, two tables are added to your database. First is the table which
  keeps track of the roles and their mandatory group lists. The second is a list of
  groups that can be used to fill those mandatory lists.
 
  Next, some prep work is done to create the pseudo-roles of 'All users' and 'Group
  admins'. If the regular og_mandatory_group was previously installed, it left
  behind a couple of variables in the variable table in your database. One of them
  will be read to see what existing mandatory group(s) you may have had and add them
  to the 'All users' list. This was tested using a patch of og_mandatory_group
  which allowed multiple mandatory groups. This may not work with the unpatched
  original module. Likewise, that same patch had a list of mandatory groups for group
  owners and this will be added to the 'Group admins' "role".

  If those variables don't exist, blank 'All users' and 'Group admins' lists will
  be created. If you have installation problems, delete the og_mandatory_group
  variables from you variable table in your database and post an issue so I know.

*Functionality*
  Provided you have defined roles, and have created groups, you follow these steps:
  1) go to admin/og/og_multiple_mandatory_groups_by_role/groups
   Here, you will add groups to an availability list. There is a drop down list
   populated with both 'open' and 'closed' groups. 'Invitation only' and 
   'moderated' groups are not allowed as the owners of those kinds of groups have
   total control over their membership and it doesn't make sense to try and make
   mandatory, groups where you can't keep track of who should be in them and who
   shouldn't.
   
   These are the groups that can be placed into mandatory lists as described below.
   The intention was for these groups to be administered solely by this module, so
   you should design and place into this list only those groups to which you will
   NOT be adding users manually. Ideally, you should use 'closed' groups, though
   for compatibiliy with og_mandatory_group, I have allowed 'open' groups as well.
   
   Notice that there are two delete links. The first removes the group from the
   availability list without unsubscribing any users. This is for when you may wish
   to delete several groups from the list in a row, without unsubscribing during
   each deletion. Instead you can update all users and groups after deleting many
   groups as explained in number 3, below.
   
   The other delete link, of course, unsubscribes the users of the group and updates
   all database entries at once.
   
   Note: Upon deleting a group node, this module responds and cleans up its database
   entries so old groups won't show up on the mandatory lists. Users are unsubscribed
   by the OG module directly.
   
  2) go to admin/og/og_multiple_mandatory_groups_by_role
   Here, you can add the roles you want to have mandatory groups attached to. There
   is a drop down list of all roles and you may add any of them. There are two
   pseudo-roles which come with the module and which cannot be deleted via this page;
   they are the 'All users' "role" and the 'Group admins' "role".
   
   The first, is of course, used to create a list of mandatory groups for all users.
   This does not depend on the default role of 'authenticated user' or any other
   actual role. All new users will be subscribed to the groups in this list as
   you define it as in number 3, below.
   
   The second is a list of mandatory groups that you can create for all group admins.
   That is, all group owners, and those they've specified as admins for their group.
   
   There are links to 'assign groups'. If you click on one of these, you will be
   taken to number 3, below.
   
   You'll notice that here too, there are two delete links. Again, the first simply
   deletes the role, but doesn't update the users' subscriptions. If you are deleting
   many roles at once, you can use this, then update all users per number 3, below.
   
   The other delete link unsubscribes users of the group(s) that belong to the role
   but will not unsubscribe users from groups that are also in another role's
   mandatory list.
   
   Note: Upon deleting a role from Drupal, this module responds by unsubscribing
   users as needed and cleaning up its database entries.
   
  3) go to admin/og/og_multiple_mandatory_groups_by_role/admin
   Here, you assign groups to roles. If you clicked on a single 'assign groups' link
   from the role management page, you will be presented with a single fieldset,
   uncollapsed, containing a checkbox listing of all groups from the group availability
   list you made earlier in number 1, above. Select as many groups as you'd like to
   make mandatory for this role.
   
   If you clicked on the 'Assign Groups' tab, you will be presented with a stack of
   collapsed fieldsets. Each one is for a different role that you've added in number
   2, above. You can open and close however many at a time you wish. You will notice
   that each has the same listing of groups from the group availability list. Click
   on any combination you wish for each role's mandatory list. You can have groups
   appear in any number of lists.
   
   In keeping with a feature from og_mandatory_group, you will find that under the
   'All users' "role", that there is a checkbox labeled 'Require One Additional Group'.
   This means that when a new user registers and is presented with a list of groups
   that they may subscribe to, that they must choose at least one of them. This is
   dependent on whether any groups have been placed into the registration list. If not,
   this probably breaks.
   
   There is a checkbox labeled 'Unsubscribe unchecked boxes'. The first version of this
   module accidentally unsubscribed users from unchecked groups for roles that they
   belonged to. Additively, this would unsubscribe them from groups that, based on a
   role that occurs alphabetically before it, they should have. This would also
   unsubscribe users if they had joined the group through other (normal) means. This
   was not desired behavior and has been fixed. However, you can now unsubscribe users
   from the groups not checked for a given role by checking this box. It may be that
   you never use this feature. As I write this, I can't recall whether this simply
   enables the former behavior, or if, by checking this box for all roles, it will
   respect all checked groups and only unassign the aggregate unchecked groups for
   each role.
   
   You will also find a checkbox marked 'update subscriptions retroactively'. This
   affects not only this role's groups, but all groups that you've decided to place
   into the care of this module. More on that in a minute.
   
   Whether you have a single role view, or the multi-fieldset view, clicking on 'Save
   assignments' will save to the database each list of mandatory groups and will
   update users who have previously been updated via this module. This point is a little
   confusing, so allow me to present an example:
   
   If you have an existing site, with existing users, roles, groups, etc. and you install
   this module, there won't be existing integrity between the mandatory lists you set up
   and the current state of who is in what group. If you only create new groups, this
   should not be a problem as performing steps 1-3 upto this point will take care of
   adding these new groups to the existing users with corresponding roles. However, if
   you were to use existing groups, with existing members, there may be some people who,
   by virtue of the fact that they don't have the role that you now require, shouldn't
   be in that group. Clicking on 'Update Subscriptions Retroactively' takes care of this.
   All users, no matter what roles they have or groups they currently belong to, will,
   when this box is checked and the page saved, be dropped from 'illegal groups' and be
   placed into the correct mandatory groups. So make sure that all users have the roles
   that they should have before doing this action.
   
   Bonus: If you ever think your group memberships are incorrect, you can come to this
   page, check the box and save the page. You need not make any changes.
   
   Warning: Depending on the number of users you have, this could be an expensive process.
   For this reason, you may wish to increase your server's time-out properties, and/or
   run this when you have little traffic. No benchmarking was done for this, so maybe
   I'm getting worried about nothing.
   
*Cooperation*
  This module plays nice with standard Drupal. The following are descriptions of what
  happens behind the scenes on two important Drupal admin pages:
  1) at admin/user/user is the multi-user administration page. Assigning a role to several
   users at once will update their mandatory groups by subscribing them to the groups in
   that role's list (if any, and if the user isn't already subscribed). Deleting a role
   from several users at once will likewise unsubscribe them from the groups of that
   role provided that they have no other roles which require that group.
   
  2) at user/XX/edit where XX is a user ID is the standard user edit screen where roles can
   be assigned. Whatever the state of the roles when this page is saved, this module
   subscribes and unsubscribes groups to end up with subscriptions that match the end
   state of the roles. This is a good place to come if you think that a single user's
   subscriptions are incorrect. You don't need to change anything, just come to the page
   and save it and the user's subscriptions will align with your mandatory settings.
   Of course roles without mandatory groups are ignored.
   
*Wrap up*
  I think that about covers it. Oh, except I should mention that email support has been
  stripped. I felt that with the number of potential changes and the number of time they
  could happen, that group owners might become annoyed by them - especially if the group
  owner is you!

  Of course, should you find any bugs, post an issue at Drupal.org. If you'd like to post
  a patch for it too, even better.

--Created by--
  Ryan Constantine (rconstantine)

--Some code borrowed from og_mandatory_group by--
  Gerhard Killesreiter (killes) [original writer] and possibly some from Peter (pwolanin)
  
--D6 conversion sponsored by www.iofc.org