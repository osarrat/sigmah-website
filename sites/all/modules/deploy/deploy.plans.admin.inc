<?php
/**
 * @file
 * Page handlers for deploy plan admin.
 */

/**
 * Display a list of all deployment plans.
 *
 * @return
 *   Themed table output.
 */
function deploy_overview() {
  $plans = deploy_get_plans();
  foreach ($plans as $plan) {
    $items = deploy_get_plan_items($plan['pid']);
    $plan_entry = l($plan['name'], 'admin/build/deploy/list/'. $plan['pid']);
    if (empty($items)) {
      $plan_entry = check_plain($plan['name']);
    }
    $row = array(
      $plan_entry,
      check_plain($plan['description']),
      l(t('edit'), 'admin/build/deploy/plan/'. $plan['pid']),
      l(t('delete'), 'admin/build/deploy/delete/plan/'. $plan['pid']),
      l(t('push'), 'admin/build/deploy/push/'. $plan['pid']),
    );
    $rows[] = $row;
  }

  if (empty($rows)) {
    $rows[] = array(array('data' => t('No deployment plans available.'), 'colspan' => '5', 'class' => 'message'));
  }

  $header = array(
    t('Name'),
    t('Description'),
    array(
      'data' => t('Operations'),
      'colspan' => 3,
    )
  );

  $output = theme('table', $header, $rows);

  $output .= l(t("Add a New Deployment Plan"), "admin/build/deploy/add", array());

  return $output;
}

/**
 * Display add/edit deployment plan form.
 *
 * @param $form_state
 *   FAPI form state
 * @param $pid
 *   Unique identifier for the plan we're editing, or NULL if creating a new plan.
 * @return
 *   FAPI array
 * @ingroup forms
 * @see deploy_plan_form_submit()
 */
function deploy_plan_form($form_state, $pid = NULL) {
  $plan = NULL;

  // If we got a PID, get the plan's details.
  if (!is_null($pid)) {
    $plan = deploy_get_plan($pid);
    $form['pid'] = array(
      '#type' => 'hidden',
      '#default_value' => $pid,
    );
  }

  $form['plan_name'] = array(
    '#title' => t('Name'),
    '#type' => 'textfield',
    '#size' => 30,
    '#maxlength' => 50,
    '#required' => TRUE,
    '#default_value' => $plan['name'],
    '#description' => t('A unique name for this deployment plan.'),
  );

  $form['plan_description'] = array(
    '#title' => t('Description'),
    '#type' => 'textarea',
    '#default_value' => $plan['description'],
    '#description' => t('Information about this deployment plan, and the items being deployed.'),
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save Deployment Plan'),
  );

  return $form;
}

/**
 * Validate callback for deploy_plan_form().
 */
function deploy_plan_form_validate($form, &$form_state) {
  // If we're adding a new plan, then check to make sure another plan
  // doesn't exist with the same name.
  if (!array_key_exists('pid', $form_state['values'])) {
    if (deploy_plan_exists($form_state['values']['plan_name'])) {
      form_set_error('plan name', 'A plan with that name already exists');
    }
  }
}

/**
 * Submit callback for deploy_plan_form().
 */
function deploy_plan_form_submit($form, &$form_state) {
  global $user;

  // If $form_state['values'] has 'pid', then this is an update. Otherwise it is an
  // insert.
  if (array_key_exists('pid', $form_state['values'])) {
    db_query("UPDATE {deploy_plan} SET name = '%s', description = '%s' WHERE pid = %d", $form_state['values']['plan_name'], $form_state['values']['plan_description'], $form_state['values']['pid']);
  }
  else {
    deploy_create_plan($form_state['values']['plan_name'], $form_state['values']['plan_description']);
  }

  $form_state['redirect'] = 'admin/build/deploy';
}

/**
 * Display a list of all items in a specified plan.
 *
 * @param $form_state
 *   FAPI form state
 * @param $pid
 *   Unique identifier for the plan we're viewing
 * @return
 *   FAPI array
 * @ingroup forms
 * @see deploy_list_form_submit()
 */
function deploy_list_form($form_state, $pid = NULL) {
  // If this is a submit for deletion, return the confirm form
  if (isset($form_state['values']['plan_items'])) {
    return deploy_list_multiple_delete_confirm($form_state, array_filter($form_state['values']['plan_items']));
  }
  
  // If there are no plan items then bail.
  $items = deploy_get_plan_items($pid);
  if (is_null($pid) || empty($items)) {
    drupal_goto('/admin/build/deploy');
  }
  
  // Set the page title to the name of the plan
  $plan = deploy_get_plan($pid);
  drupal_set_title(t('Deployment plan items - @plan', array('@plan' => $plan['name'])));

  // username cache so we don't have to user_load constantly
  $user_cache = array();

  // Build form tree
  $form['#tree'] = TRUE;
  $plan_items = array();
  foreach ($items as $i => $item) {
    if (!array_key_exists($item['uid'], $user_cache)) {
      $account = user_load(array('uid' => $item['uid']));
      $user_cache[$item['uid']] = $account->name;
    }
    $added_by = $user_cache[$item['uid']] ." on ". format_date($item['ts'], 'small');
    $form[$i]['module'] = array('#type' => 'item', '#value' => $item['module']);
    $form[$i]['description'] = array('#type' => 'item', '#value' => $item['description']);
    $form[$i]['added_by'] = array('#type' => 'item', '#value' => $added_by);
    $form[$i]['iid'] = array('#type' => 'hidden', '#value' => $item['iid']);
    $plan_items[$item['iid']] = '';
  }

  $form['plan_items'] = array('#type' => 'checkboxes', '#options' => $plan_items);
  $form['pid'] = array('#type' => 'hidden', '#value' => $pid);
  $form['submit'] = array('#type' => 'submit', '#value' => t('Remove items from plan'));

  return $form;
}

/**
 * Submit callback for deploy_list_form().
 */
function deploy_list_form_submit($form, &$form_state) {
  $form_state['rebuild'] = TRUE;
}

/**
 * Confirmation form for deploy plan item deletes.
 *
 * @param $form_state
 *   FAPI form state
 * @param $items
 *   The items tp be deleted
 * @return
 *   FAPI array
 * @ingroup forms
 * @see deploy_list_multiple_delete_confirm()
 */
function deploy_list_multiple_delete_confirm($form_state, $items) {
  $form['items'] = array('#prefix' => '<ul>', '#suffix' => '</ul>', '#tree' => TRUE);

  foreach ($items as $iid => $value) {
    $title = db_result(db_query('SELECT description FROM {deploy_plan_items} WHERE iid = %d', $iid));
    $form['items'][$iid] = array(
      '#type' => 'hidden',
      '#value' => $iid,
      '#prefix' => '<li>',
      '#suffix' => check_plain($title) ."</li>\n",
    );
  }
  $form['operation'] = array('#type' => 'hidden', '#value' => 'delete');
  $form['#submit'][] = 'deploy_list_multiple_delete_confirm_submit';

  return confirm_form($form,
                      t('Are you sure you want to remove these items from the deployment plan?'),
                      'admin/content/node', t('This action cannot be undone.'),
                      t('Remove all'), t('Cancel'));
}

/**
 * Form submit callback for deploy_list_multiple_delete_confirm().
 */
function deploy_list_multiple_delete_confirm_submit($form, &$form_state) {
  if ($form_state['values']['confirm']) {
    foreach ($form_state['values']['items'] as $iid => $value) {
      deploy_plan_item_delete(array('iid' => $iid));
    }
    drupal_set_message(t('The plan items have been removed.'));
  }
  $form_state['redirect'] = 'admin/build/deploy';
}

/**
 * Implementation of hook_theme().
 */
function theme_deploy_list_form($form) {

  $header = array(theme('table_select_header_cell'), t('Module'), t('Description'), t('Added by'));
  $rows = array();

  foreach (element_children($form) as $i) {
    $item =& $form[$i];
    if (is_array($item['module'])) {
      $rows[] = array(
        drupal_render($form['plan_items'][$item['iid']['#value']]),
        drupal_render($item['module']),
        drupal_render($item['description']),
        drupal_render($item['added_by']),
      );
    }
  }
  
  $output = theme('table', $header, $rows, array('id' => 'plans'));
  $output .= drupal_render($form);

  return $output;
}

/**
 * Delete a deployment plan confirmation form.
 *
 * @param $form_state
 *   FAPI form state
 * @param $pid
 *   Unique identifier of the plan we're deleting.
 * @return
 *   FAPI form definition
 * @ingroup forms
 * @see deploy_delete_plan_form_submit()
 */
function deploy_delete_plan_form($form_state, $pid = NULL) {
  $plan = deploy_get_plan($pid);
  if (!$plan) {
    drupal_goto('admin/build/deploy');
  }

  $form['pid'] = array('#type' => 'value', '#value' => $pid);

  $form = confirm_form(
    $form,
    t('Are you sure you want to delete the deployment plan %plan_name?', array('%plan_name' => $plan['name'])),
    'admin/build/deploy',
    t('This action cannot be undone.'),
    t('Delete'),
    t('Cancel')
  );

  return $form;
}

/**
 * Submit callback function for deploy_delete_plan_form();
 */
function deploy_delete_plan_form_submit($form, &$form_state) {
  db_query("DELETE FROM {deploy_plan_items} WHERE pid = %d", $form_state['values']['pid']);
  db_query("DELETE FROM {deploy_plan} WHERE pid = %d", $form_state['values']['pid']);

  drupal_set_message(t('Plan Deleted'));

  drupal_goto("/admin/build/deploy/");
}
