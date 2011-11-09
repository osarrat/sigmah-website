<?php
// $Id:
/**
 * @file
 * Deployment API which enables modules to deploy items between servers.
 *
 * This module manages deployment-related issues for comments.
 */

/**
 * Display comment "add to plan" form.
 *
 * @param $form_state
 *   FAPI form state
 * @param $cids
 *   The cids of the comments we're attempting to deploy.
 * @return
 *   FAPI array
 * @ingroup forms
 * @see comment_deploy_operations_add_form_submit()
 */
function comment_deploy_operations_add_form($form_state, $cids) {
  // If no comments selected then bail.
  if (!$cids) {
    drupal_set_message('No comments selected for deployment.');
    drupal_goto('admin/content/comment');
  }

  $form['pid'] = array(
    '#title' => t('Deployment Plan'),
    '#type' => 'select',
    '#options' => deploy_get_plan_options(),
    '#required' => TRUE,
    '#description' => t('The deployment plan to add these comments to.'),
  );

  $form['cids'] = array(
    '#type' => 'hidden',
    '#default_value' => $cids,
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Submit'),
  );

  return $form;
}

/**
 * Submit handler for comment_deploy_operations_add_form().
 */
function comment_deploy_operations_add_form_submit($form, &$form_state) {
  $result = db_query("SELECT cid, subject FROM {comments} WHERE cid IN (%s)", $form_state['values']['cids']);
  $pid = $form_state['values']['pid'];

  while ($row = db_fetch_array($result)) {
    if (!deploy_item_is_in_plan($pid, 'comment', $row['cid'])) {
      deploy_add_to_plan($pid, 'comment', $row['subject'], $row['cid'], 0, DEPLOY_COMMENT_GROUP_WEIGHT);
    }
  }

  $form_state['redirect'] = "admin/build/deploy/list/$pid";
}

/**
 * Display comment "deploy now" form.
 *
 * @param $form_state
 *   FAPI form state
 * @param $cids
 *   The cids of the comments we're attempting to deploy.
 * @return
 *   FAPI array
 * @ingroup forms
 * @see comment_deploy_operations_add_now_form_submit()
 */
function comment_deploy_operations_add_now_form($form_state, $cids) {
  if (!$cids) {
    drupal_set_message('No comments selected for deployment.');
    drupal_goto('admin/content/comment');
  }

  $form = deploy_get_server_form();

  $form['cids'] = array(
    '#type' => 'hidden',
    '#default_value' => $cids,
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Submit'),
  );

  return $form;
}

/**
 * Submit handler for comment_deploy_operations_add_now_form().
 */
function comment_deploy_operations_add_now_form_submit($form, &$form_state) {
  $sid = $form_state['values']['sid'];

  // If there is already a plan for these comments, then empty it and use it.
  // Otherwise create one. Every deployment needs to be attached to a plan, so
  // under the hood we make a plan called 'Comments Now' and mark it as internal
  // only, which forces it to be hidden from the user-facing deployment plan admin.
  $pid = deploy_plan_exists('Comments Now');
  if (!$pid) {
    $pid = deploy_create_plan('Comments Now', 'Internal plan to deploy comments', 1);
  }
  else {
    deploy_empty_plan($pid);
  }

  $result = db_query("SELECT cid, subject FROM {comments} WHERE cid IN (%s)", $form_state['values']['cids']);
  while ($row = db_fetch_array($result)) {
    if (!deploy_item_is_in_plan($pid, 'comment', $row['cid'])) {
      deploy_add_to_plan($pid, 'comment', $row['subject'], $row['cid'], 0, DEPLOY_COMMENT_GROUP_WEIGHT);
    }
  }

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
