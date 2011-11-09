<?php
// $Id: webfm.admin.inc,v 1.4 2009/08/19 15:33:48 robmilne Exp $

/**
 * @file
 * Provides the administration page for Web File Manager.
 */

/**
 * Implementation of hook_settings().
 */
function webfm_admin_settings() {
  global $user;

  $modulepath = drupal_get_path('module', 'webfm');

  $form['webfm_root_dir'] =
    array('#type' => 'textfield',
          '#title' => t('WebFM root directory'),
          '#default_value' => variable_get('webfm_root_dir', ''),
          '#maxlength' => '100',
          '#size' => '70',
          '#description' => t('Root directory used to present the filebrowser user interface.
                               <br />This path is relative to "File system path" set in admin/settings/file-system.
                               <br />If this directory path is compound (ie: path/to/root) then the path must already
                               <br />exist for this setting to validate (ie: path/to).')

          );
  $form['webfm_icon_dir'] =
    array('#type' => 'textfield',
          '#title' => t('Icon directory'),
          '#default_value' => variable_get('webfm_icon_dir', $modulepath. '/image/icon'),
          '#maxlength' => '100',
          '#size' => '70',
          '#description' => t('Name of directory where file type icons are stored (relative to base url).')
         );
  $form['webfm_max_resolution'] =
    array('#type' => 'textfield',
          '#title' => t('Maximum resolution for uploaded images'),
          '#default_value' => variable_get('webfm_max_resolution', 0),
          '#size' => 15,
          '#maxlength' => 10,
          '#description' => t('The maximum allowed image size (e.g. 640x480). Set to 0 for no restriction.'),
          '#field_suffix' => '<kbd>'. t('WIDTHxHEIGHT') .'</kbd>'
          );

  $form['webfm_date_format'] =
    array('#type' => 'radios',
          '#title' => t('Date Format'),
          '#default_value' => variable_get('webfm_date_format', WEBFM_DATE_FORMAT_DAY),
          '#options' => array(WEBFM_DATE_FORMAT_DAY => t('dd/mm/yy'), WEBFM_DATE_FORMAT_MONTH => t('mm/dd/yy')),
          '#description' => t('Set the order of day/month in date fields.')
         );

  $form['webfm_display_title'] =
    array('#type' => 'checkbox',
          '#title' => t('Display metadata title'),
          '#default_value' => variable_get('webfm_display_title', ''),
          '#description' => t('Check this box to display the metadata title in the browser instead of the filename.
                               <br />If a metadata title doesn\'t exist, the filename is used and rename functions normally.
                               <br />If metadata title is used, renaming is available inside the metadata editor.')
         );

  $form['webfm_display_owner'] =
    array('#type' => 'checkbox',
          '#title' => t('Display file owner column in browser'),
          '#default_value' => variable_get('webfm_display_owner', ''),
          '#description' => t('Check this box to display the file owner column in the browser.')
         );

  $form['file_perm'] =
    array('#type' => 'fieldset',
          '#title' => t('Default File Permissions'),
          '#collapsible' => TRUE,
          '#collapsed' => TRUE,
          '#description' => t('Default permissions applied to a file when inserted into the database.
                               <br />If all boxes are unchecked only the file owner and administrators will have
                               <br />access to the file via the filebrowser.  File owners and admins can change
                               <br />these settings for each individual file within the filebrowser UI.')
          );

  $form['file_perm']['webfm_file_perm_role'] =
    array('#type' => 'checkbox',
          '#title' => t('Role View Access'),
          '#default_value' => variable_get('webfm_file_perm_role', ''),
          '#description' => t('File is viewable/downloadable in the filebrowser by role.')
          );

  $form['file_perm']['webfm_file_perm_mod'] =
    array('#type' => 'checkbox',
          '#title' => t('Role Full Access'),
          '#default_value' => variable_get('webfm_file_perm_mod', ''),
          '#description' => t('Roles that can access this file via the filebrowser have the same access rights as the
                              <br />file owner except for the right to change the file permissions.')
          );

  $form['file_perm']['webfm_file_perm_attach'] =
    array('#type' => 'checkbox',
          '#title' => t('Role Attach Access'),
          '#default_value' => variable_get('webfm_file_perm_attach', ''),
          '#description' => t('Roles that can access this file via the filebrowser have the right to attach it to content.')
          );

  $form['file_perm']['webfm_file_public'] =
    array('#type' => 'checkbox',
          '#title' => t('Public Access'),
          '#default_value' => variable_get('webfm_file_public', ''),
          '#description' => t('File is downloadable anonymously via webfm_send.')
          );

  $roles = user_roles(0,'access webfm');
  $form['roles'] = array('#type' => 'value', '#value' => $roles);

  // Flush extensions regex cache for upload and db enum
  webfm_get_extensions_regex(WEBFM_FLUSH);

  foreach ($roles as $rid => $role) {
    if(variable_get("root_dir_".$rid, '') == '') {
      drupal_set_message(t('Role root directory not set for @role role', array('@role' => $role)), 'error');
    }

    $form["settings_role_".$rid] =
      array('#type' => 'fieldset',
            '#title' => t('Settings for @role role', array('@role' => $role)),
            '#collapsible' => TRUE,
            '#collapsed' => TRUE
            );

    $form["settings_role_".$rid]["root_dir_".$rid] =
      array('#type' => 'textfield',
            '#title' => t('Role Root directory'),
            '#default_value' => variable_get("root_dir_".$rid, ''),
            '#maxlength' => '100',
            '#size' => '70',
            '#description' => t('Root directory for this role.
                                <br />This path is relative to "WebFM Root directory" set above.
                                <br />If this directory path is compound then the path must already exist for this
                                <br />setting to validate.')
           );

    $form["settings_role_".$rid]["webfm_extensions_".$rid] =
      array('#type' => 'textfield',
            '#title' => t('Permitted file extensions'),
            '#default_value' => variable_get("webfm_extensions_".$rid, "jpg jpeg gif png txt html htm doc xls pdf ppt pps dwg zip gz"),
            '#maxlength' => 255,
            '#description' => t('Extensions that users in this role can upload. Separate extensions with a space
                                 <br />and do not include the leading dot.')
            );

    $form["settings_role_".$rid]["webfm_uploadsize_".$rid] =
      array('#type' => 'textfield',
            '#title' => t('Maximum file size per upload'),
            '#default_value' => variable_get("webfm_uploadsize_".$rid, 4),
            '#size' => 5,
            '#maxlength' => 5,
            '#description' => t('The maximum size of a file a user can upload (in megabytes).
                                 <br />Cannot exceed %size limit set in php.ini.', array('%size' => format_size(file_upload_max_size())))
            );
    $form["settings_role_".$rid]["webfm_usersize_".$rid] =
      array('#type' => 'textfield',
            '#title' => t('Total file size per user'),
            '#default_value' => variable_get("webfm_usersize_".$rid, 200),
            '#size' => 5,
            '#maxlength' => 5,
            '#description' => t('The maximum size of all files a user can have on the site (in megabytes).')
            );
  }

  if (module_exists('og')) {
    // Per group settings
    $groups = og_all_groups_options();
    $form['groups'] = array('#type' => 'value', '#value' => $groups);
    $form['webfm_og_auto'] = array(
      '#type' => 'checkbox',
      '#default_value' => variable_get('webfm_og_auto', 0),
      '#title' => t('Create new group folders'),
      '#description' => t('Automatically create a new group folder when a group is created.'),
    );
    foreach ($groups as $gid => $group) {
      if (variable_get("root_dir_group_".$gid, '') == '') {
        drupal_set_message(t('Group root directory not set for @group group', array('@group' => $group)), 'error');
      }
      $form["settings_group_". $gid] =
        array('#type' => 'fieldset',
              '#title' => t('Settings for @group group', array('@group' => $group)),
              '#collapsible' => TRUE,
              '#collapsed' => TRUE
             );
      $form["settings_group_". $gid]["root_dir_group_". $gid] =
        array('#type' => 'textfield',
              '#title' => t('Group Root directory'),
              '#default_value' => variable_get("root_dir_group_". $gid, ''),
              '#maxlength' => '100',
              '#size' => '70',
              '#description' => t('Root directory for this group.
                                   <br />This path is relative to "WebFM Root directory" set above.
                                   <br />If this directory path is compound then the path must already exist for this
                                   <br />setting to validate.'),
             );
    }
  }


  $form['attach'] =
    array('#type' => 'fieldset',
          '#title' => t('WebFM attachments'),
          '#collapsible' => TRUE,
          '#collapsed' => TRUE
          );
  $form['attach']['webfm_attach_body'] =
    array('#type' => 'checkbox',
          '#title' => t('Append file links to Node Body'),
          '#default_value' => variable_get('webfm_attach_body', ''),
          '#description' => t('Check this box to append file attachments table to the node body.
                               <br />This setting does not affect the attachment block.')
          );
  $form['attach']['webfm_attach_new_window'] =
    array('#type' => 'checkbox',
          '#title' => t('Open attachments in a new browser window'),
          '#default_value' => variable_get('webfm_attach_new_window', '')
          );
  $form['attach']['attrib'] =
    array('#type' => 'fieldset',
          '#title' => t('Attachment List Properties'),
          );
  $form['attach']['attrib']['webfm_attach_desc'] =
    array('#type' => 'checkbox',
          '#title' => t('Include file description metadata'),
          '#default_value' => variable_get('webfm_attach_desc', ''),
          '#description' => t('Check this box to add file description metadata beneath the attachment title.')
          );
  $form['attach']['attrib']['webfm_attach_date'] =
    array('#type' => 'checkbox',
          '#title' => t('Enable file date column'),
          '#default_value' => variable_get('webfm_attach_date', ''),
          '#description' => t('Check this box to add a create date column to the attachment table.')
          );
  $form['attach']['attrib']['webfm_attach_size'] =
    array('#type' => 'checkbox',
          '#title' => t('Enable file size column'),
          '#default_value' => variable_get('webfm_attach_size', ''),
          '#description' => t('Check this box to add a file size column to the attachment table.')
          );
  if($user->uid == 1) {
    //only user #1 gets this access
    $form['ie'] =
      array('#type' => 'fieldset',
            '#title' => t('IE Drag-and-Drop Normalization'),
            '#collapsible' => TRUE,
            '#collapsed' => TRUE,
            '#description' => t('number of pixels to offset drag objects from cursor position in IE browser.<br />This quantity is usually related to relative positioning used by the css.')
            );
    $form['ie']['webfm_ie_dd_list_offset'] =
      array('#type' => 'textfield',
            '#title' => t('IE drag and drop x-axis offset for right hand listing draggables'),
            '#default_value' => variable_get('webfm_ie_dd_list_offset', '-190'),
            '#maxlength' => '10',
            '#size' => '10',
            );
    $form['ie']['webfm_ie_dd_tree_offset'] =
      array('#type' => 'textfield',
            '#title' => t('IE drag and drop x-axis offset for directory tree draggables'),
            '#default_value' => variable_get('webfm_ie_dd_tree_offset', '-34'),
            '#maxlength' => '10',
            '#size' => '10',
            );

  $form['debug'] =
    array('#type' => 'fieldset',
          '#title' => t('WebFM debug'),
          '#collapsible' => TRUE,
          '#collapsed' => TRUE
          );
  $form['debug']['webfm_debug'] =
    array('#type' => 'checkbox',
          '#title' => t('WebFM javascript debug'),
          '#default_value' => variable_get('webfm_debug', ''),
          '#description' => t('Check this box for javascript debug messaging.')
          );
    $form['debug']['webfm_cron'] =
      array('#type' => 'checkbox',
            '#title' => t('WebFM cron'),
            '#default_value' => variable_get('webfm_cron', ''),
            '#description' => t('Check this box to enable cleanup of orphaned file records in the database.
                                 <br />NOTE: Use with caution - behaviour is to delete all file records without a
                                 <br />valid file path.  Manually renaming a WebFM directory (ie: via OS shell)
                                 <br />and then running cron will delete all webfm_file table entries for that
                                 <br />directory and it\'s sub-directories.')
            );
  }

  $form['#validate'] = array('webfm_admin_settings_validate');
  return system_settings_form($form);
}

/**
 * Form API callback to validate the webfm settings form.
 */
function webfm_admin_settings_validate($form, $form_state) {
  $valid_webfm_root = FALSE;
  $webfm_root_dir_name = $form_state['values']['webfm_root_dir'];
  if(!empty($webfm_root_dir_name)) {
    if(!preg_match('/^[0-9a-zA-Z]/', $webfm_root_dir_name)) {
      form_set_error('webfm_root_dir'. $rid, t('The leading character of the webfm root directory name must be alphanumeric.'));
    } else if(preg_match('[\.]', $webfm_root_dir_name)) {
      form_set_error('webfm_root_dir'. $rid, t('The webfm root directory name is not valid.'));
    } else {
      $webfm_root_dir = file_directory_path()."/".$webfm_root_dir_name;
      $valid_webfm_root = file_check_directory($webfm_root_dir, FILE_CREATE_DIRECTORY, 'webfm_root_dir');
    }
  }

  if(($form_state['values']['webfm_max_resolution'] != '0')) {
    if(!preg_match('/^[0-9]+[xX][0-9]+$/', $form_state['values']['webfm_max_resolution'])) {
      form_set_error('webfm_max_resolution', t('The maximum allowed image size expressed as WIDTHxHEIGHT (e.g. 640x480). Set to 0 for no restriction.'));
    }
  }

  $exceed_max_msg = t('Your PHP settings limit the maximum file size per upload to %size MB.', array('%size' => file_upload_max_size())).'<br/>';
  $more_info = t("Depending on your sever environment, these settings may be changed in the system-wide php.ini file, a php.ini file in your Drupal root directory, in your Drupal site's settings.php file, or in the .htaccess file in your Drupal root directory.");
  $max_upload_size = file_upload_max_size()/(1024*1024);
  foreach($form_state['values']['roles'] as $rid => $role)  {
    $uploadsize = $form_state['values']['webfm_uploadsize_'. $rid];
    $usersize = $form_state['values']['webfm_usersize_'. $rid];
    $role_root_dir_name = $form_state['values']['root_dir_'.$rid];

    if(!empty($role_root_dir_name)) {
      if($valid_webfm_root) {
        if(!preg_match('/^[0-9a-zA-Z]/', $role_root_dir_name)) {
          form_set_error('root_dir_'. $rid, t('The leading character of the %role root directory must be alphanumeric.', array('%role' => $role)));
        } else if(preg_match('[\.]', $role_root_dir_name)) {
          form_set_error('root_dir_'. $rid, t('The %role root directory name is not valid.', array('%role' => $role)));
        } else {
          $role_root_dir = $webfm_root_dir."/".$role_root_dir_name;
          file_check_directory($role_root_dir, FILE_CREATE_DIRECTORY, 'root_dir_'.$rid);
        }
      } else {
        form_set_error('root_dir_'. $rid, t('The WebFM root directory must be valid for the %role root directory name to be valid.', array('%role' => $role)));
      }
    }

    if(!is_numeric($uploadsize) || ($uploadsize <= 0)) {
      form_set_error('webfm_uploadsize_'. $rid, t('The %role file size limit must be a number and greater than zero.', array('%role' => $role)));
    }
    if(!is_numeric($usersize) || ($usersize <= 0)) {
      form_set_error('webfm_usersize_'. $rid, t('The %role file size limit must be a number and greater than zero.', array('%role' => $role)));
    }
    if($uploadsize > $max_upload_size) {
     form_set_error('webfm_uploadsize_'. $rid, $exceed_max_msg . $more_info);
     $more_info = '';
    }
    if($uploadsize > $usersize) {
     form_set_error('webfm_uploadsize_'. $rid, t('The %role maximum file size per upload is greater than the total file size allowed per user', array('%role' => $role)));
    }
  }

  if (module_exists('og')) {
    foreach ($form_state['values']['groups'] as $gid => $group)  {
      $group_root_dir_name = $form_state['values']['root_dir_group_'. $gid];
      if (!empty($group_root_dir_name)) {
        if ($valid_webfm_root) {
          if (!preg_match('/^[0-9a-zA-Z]/', $group_root_dir_name)) {
            form_set_error('root_dir_group_'. $gid, t('The leading character of the %group root directory must be alphanumeric.', array('%group' => $group)));
          } else if(preg_match('[\.]', $group_root_dir_name)) {
            form_set_error('root_dir_group_'. $gid, t('The %group root directory name is not valid.', array('%group' => $group)));
          } else {
            $group_root_dir = $webfm_root_dir ."/". $group_root_dir_name;
            file_check_directory($group_root_dir, FILE_CREATE_DIRECTORY, 'root_dir_group_'.$gid);
          }
        } else {
          form_set_error('root_dir_group_'. $gid, t('The WebFM root directory must be valid for the %group root directory name to be valid.', array('%group' => $group)));
        }
      }
    }
  }
}
