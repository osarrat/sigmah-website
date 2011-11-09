<?php
/**
 * @file
 * Page handlers for node deployment pages.
 */

/**
 * Display node "add to plan" form.
 *
 * @param $form_state
 *   FAPI form state
 * @param $node
 *   The node we're attempting to deploy.
 * @return
 *   FAPI array
 * @ingroup forms
 * @see node_deploy_add_form_submit()
 */
function node_deploy_add_form($form_state, $node) {
  $options = deploy_get_plan_options();
  if (!empty($options)) {
    $form['pid'] = array(
      '#title' => t('Deployment Plan'),
      '#type' => 'select',
      '#options' => $options,
      '#required' => TRUE,
      '#description' => t('The deployment plan to add this !type to.', array('!type' => $node->type)),
    );
    $form['nid'] = array(
      '#type' => 'hidden',
      '#default_value' => $node->nid,
    );
  
    // This is kind of a hack, but it allows us to get the title into the
    // deployment plan without a node_load() or query on the other side.
    $form['title'] = array(
      '#type' => 'hidden',
      '#default_value' => $node->title,
    );
  
    $form['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Submit'),
    );
    return $form;
  }
  return array('empty' => array('#value' => t('You currently do not have any plans set up.')));
}

/**
 * Submit handler for node_deploy_add_form().
 */
function node_deploy_add_form_submit($form, &$form_state) {
  $pid = $form_state['values']['pid'];
  $nid = $form_state['values']['nid'];
  $title = $form_state['values']['title'];

  if (!deploy_item_is_in_plan($pid, 'node', $nid)) {
    deploy_add_to_plan($pid, 'node', 'Node: '. $title, $nid, 0, DEPLOY_NODE_GROUP_WEIGHT);
  }
  $form_state['redirect'] = "admin/build/deploy/list/$pid";
}

/**
 * Display node "add to plan" form.
 *
 * @param $form_state
 *   FAPI form state
 * @param $nids
 *   The nids of the nodes we're attempting to deploy.
 * @return
 *   FAPI array
 * @ingroup forms
 * @see node_deploy_add_form_submit()
 */
function node_deploy_operations_add_form($form_state, $nids) {
  // If no nodes selected then bail.
  if (!$nids) {
    drupal_set_message('No nodes selected for deployment.');
    drupal_goto('admin/content/node');
  }

  $form['pid'] = array(
    '#title' => t('Deployment Plan'),
    '#type' => 'select',
    '#options' => deploy_get_plan_options(),
    '#required' => TRUE,
    '#description' => t('The deployment plan to add these nodes to.'),
  );

  $form['nids'] = array(
    '#type' => 'hidden',
    '#default_value' => $nids,
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Submit'),
  );

  return $form;
}

/**
 * Submit handler for node_deploy_operations_add_form().
 */
function node_deploy_operations_add_form_submit($form, &$form_state) {
  $result = db_query("SELECT nid, title, type FROM {node} WHERE nid IN (%s)", $form_state['values']['nids']);
  $pid = $form_state['values']['pid'];

  while ($row = db_fetch_array($result)) {
    if (!deploy_item_is_in_plan($pid, 'node', $row['nid'])) {
      deploy_add_to_plan($pid, 'node', $row['type'] .': '. $row['title'], $row['nid'], 0, DEPLOY_NODE_GROUP_WEIGHT);
    }
  }

  $form_state['redirect'] = "admin/build/deploy/list/$pid";
}

/**
 * Display node "deploy now" form.
 *
 * @param $form_state
 *   FAPI form state
 * @param $nids
 *   The nids of the nodes we're attempting to deploy.
 * @return
 *   FAPI array
 * @ingroup forms
 * @see node_deploy_add_now_form_submit()
 */
function node_deploy_operations_add_now_form($form_state, $nids) {
  if (!$nids) {
    drupal_set_message('No nodes selected for deployment.');
    drupal_goto('admin/content/node');
  }

  $form = deploy_get_server_form();

  $form['nids'] = array(
    '#type' => 'hidden',
    '#default_value' => $nids,
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Submit'),
  );

  return $form;
}

/**
 * Submit handler for node_deploy_operations_add_now_form().
 */
function node_deploy_operations_add_now_form_submit($form, &$form_state) {
  $sid = $form_state['values']['sid'];
  $pid = node_deploy_operations_create_plan($form_state['values']['nids']);

  // Now do the deployment
  if (deploy_plan_init($pid, $sid, $form_state['values'])) {
    // Redirect to the log overview page for this push.
    $form_state['redirect'] = "admin/build/deploy/deploy_check_batch";
  }
  else {
    $dlid = variable_get('deploy_log_id', '');
    deploy_plan_cleanup();

    // Redirect to the log overview page for this push.
    $form_state['redirect'] = "admin/build/deploy/logs/details/$dlid";    
  }
}

/**
 * Helper function that creates a deployment plan.
 */
function node_deploy_operations_create_plan($nids) {
  // If there is already a plan for this, then empty it and use it.
  // Otherwise create one.
  $pid = deploy_plan_exists('Nodes Now');
  if (!$pid) {
    $pid = deploy_create_plan('Nodes Now', 'Internal plan to deploy nodes', 1);
  }
  else {
    deploy_empty_plan($pid);
  }

  $result = db_query("SELECT nid, title, type FROM {node} WHERE nid IN (%s)", $nids);
  while ($row = db_fetch_array($result)) {
    if (!deploy_item_is_in_plan($pid, 'node', $row['nid'])) {
      deploy_add_to_plan($pid, 'node', $row['type'] .': '. $row['title'], $row['nid'], 0, DEPLOY_NODE_GROUP_WEIGHT);
    }
  }
  return $pid;
}