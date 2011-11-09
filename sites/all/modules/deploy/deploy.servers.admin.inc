<?php
/**
 * @file
 * Page handlers for deploy server admin.
 */

/**
 * Display a list of all deployment servers.
 *
 * @return
 *   Themed table output.
 */
function deploy_server_overview() {
  $header = array(t('Name'), t('URL'), t('Authentication type'), array('data' => t('Operations'), 'colspan' => 2));

  $result = db_query("SELECT * FROM {deploy_servers}");
  while ($row = db_fetch_array($result)) {
    $auth_type = deploy_get_auth_type($row['auth_type']);
    $row = array(check_plain($row['description']), check_plain($row['url']), $auth_type['title'], l(t('edit'), 'admin/build/deploy/server/'. $row['sid']), l(t('delete'), 'admin/build/deploy/delete/server/'. $row['sid']));
    $rows[] = $row;
  }

  if (empty($rows)) {
    $rows[] = array(array('data' => t('No deployment servers available.'), 'colspan' => '4', 'class' => 'message'));
  }

  $output = theme('table', $header, $rows);

  $output .= l(t("Add a new server"), "admin/build/deploy/server/add", array());

  return $output;
}

/**
 * Display add/edit deployment server form.
 *
 * @param $form_state
 *   FAPI form state
 * @param $sid
 *   Unique identifier for the server we're editing, or NULL if creating a new server.
 * @return
 *   FAPI form definition
 * @ingroup forms
 * @see deploy_server_form_validate()
 * @see deploy_server_form_submit()
 */
function deploy_server_form($form_state, $sid = NULL) {
  $server = NULL;

  // If we got a SID, get the plan's details.
  if (!empty($sid)) {
    $server = deploy_get_server($sid);
    $form['sid'] = array(
      '#type' => 'hidden',
      '#default_value' => $sid,
    );
  }

  $form['description'] = array(
    '#title' => t('Name'),
    '#type' => 'textfield',
    '#size' => 50,
    '#maxlength' => 100,
    '#required' => TRUE,
    '#default_value' => $server['description'],
    '#description' => t('Description of this server.'),
  );
  $form['url'] = array(
    '#title' => t('URL'),
    '#type' => 'textfield',
    '#size' => 50,
    '#maxlength' => 100,
    '#required' => TRUE,
    '#default_value' => $server['url'],
    '#description' => t('Domain name and path to xmlrpc service.'),
  );

  $auth_options = array('' => t('-- Select a type'));
  $auth_types = deploy_get_auth_types();
  foreach ($auth_types as $key => $type) {
    $auth_options[$key] = $type['title'];
  }

  $form['auth_type'] = array(
    '#title' => t('Authentication type'),
    '#description' => t('Select which authentication type to be used when deploying to this specific server.'),
    '#type' => 'select',
    '#required' => TRUE,
    '#options' => $auth_options,
    '#default_value' => $server['auth_type'],
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save Deployment Server'),
  );

  return $form;
}

/**
 * Submit callback for deploy_server_form().
 */
function deploy_server_form_submit($form, &$form_state) {
  $url = $form_state['values']['url'];
  $description = $form_state['values']['description'];
  $auth_type = $form_state['values']['auth_type'];

  // If 'sid' exists in the submitted form, then this is an edit. Otherwise it is
  // a new server.
  if (array_key_exists('sid', $form_state['values'])) {
    $sid = $form_state['values']['sid'];
    db_query("UPDATE {deploy_servers} SET description = '%s', url = '%s', auth_type = '%s' WHERE sid = %d", $description, $url, $auth_type, $sid);
    drupal_set_message(t('Deployment server updated'));
  }
  else {
    db_query("INSERT INTO {deploy_servers} (description, url, auth_type) VALUES ('%s', '%s', '%s')", $description, $url, $auth_type);
    drupal_set_message(t('Deployment server added'));
  }

  $form_state['redirect'] = 'admin/build/deploy/servers';
}

/**
 * Delete a deployment server confirmation form.
 *
 * @param $form_state
 *   FAPI form state
 * @param $sid
 *   Unique identifier of the server we're deleting.
 * @return
 *   FAPI form definition
 * @ingroup forms
 * @see deploy_delete_server_form_submit()
 */
function deploy_delete_server_form($form_state, $sid = NULL) {
  $server = deploy_get_server($sid);
  if (!$server) {
    drupal_goto('admin/build/deploy/servers');
  }

  $form['sid'] = array('#type' => 'value', '#value' => $sid);

  $form = confirm_form(
    $form,
    t('Are you sure you want to delete the server %server_name?', array('%server_name' => $server['description'])),
    'admin/build/deploy/servers',
    t('This action cannot be undone.'),
    t('Delete'),
    t('Cancel')
  );

  return $form;
}

/**
 * Submit callback function for deploy_delete_server_form().
 */
function deploy_delete_server_form_submit($form, &$form_state) {
  db_query("DELETE FROM {deploy_servers} WHERE sid = %d", $form_state['values']['sid']);

  drupal_set_message(t('Server Deleted'));

  drupal_goto("/admin/build/deploy/servers");
}
