<?php
/**
 * @file
 * Page handlers for content type deployment pages.
 */

/**
 * Deploy content type form.
 *
 * @param $form_state
 *   FAPI form state
 * @param $vid
 *   The vid of the vocabulary we want to deploy.
 * @return
 *   FAPI array
 * @ingroup forms
 * @see content_copy_deploy_add_form_submit()
 */
function content_copy_deploy_add_form($form_state, $plans) {
    $types = content_copy_types();

    $form['pid'] = array(
      '#title' => t('Deployment Plan'),
      '#type' => 'select',
      '#options' => $plans,
      '#description' => t('The deployment plan to add this content type to'),
    );
    $form['content_type'] = array(
      '#title' => t('Content Type'),
      '#type' => 'select',
      '#multiple' => TRUE,
      '#options' => $types,
      '#required' => TRUE,
      '#description' => t('The content type to deploy'),
    );
    $form['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Add Content Type'),
    );
    return $form;
}

/**
 * Deploy content type form submit callback.
 */
function content_copy_deploy_add_form_submit($form, &$form_state) {
  $pid = $form_state['values']['pid'];
  $module = 'content_copy';

  foreach ($form_state['values']['content_type'] as $content_type) {
    $description = "Content Type: $content_type";
    deploy_add_to_plan($pid, $module, $description, $content_type, 0, DEPLOY_CONTENT_TYPE_GROUP_WEIGHT);
  }

  $form_state['redirect'] = "admin/build/deploy/list/$pid";
}

/**
 * Deploy content type form validation callback.
 */
function content_copy_deploy_add_form_validate($form, &$form_state) {
  foreach ($form_state['values']['content_type'] as $content_type) {
    $result = db_query("SELECT iid FROM {deploy_plan_items} WHERE module = 'content_copy' AND pid = %d AND data = '%s'", $form_state['values']['pid'], $content_type);
    if (db_result($result)) {
      form_set_error('content_type', t("Content type $content_type already exists in the deployment plan."));
    }
  }
}

