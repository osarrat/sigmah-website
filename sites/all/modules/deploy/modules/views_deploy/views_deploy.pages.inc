<?php
/**
 * @file
 * Page handlers for content type deployment pages.
 */

/**
 * Display view add to deployment plan form.
 *
 * @param $form_state
 *   FAPI form state
 * @return
 *   FAPI array
 * @ingroup forms
 * @see views_deploy_add_form_submit()
 */
function views_deploy_add_form($form_state) {
  $views = array();
  $plans = deploy_get_plan_options();

  $result = db_query("SELECT name FROM {views_view} ORDER BY name");
  while ($view = db_fetch_array($result)) {
    $views[$view['name']] = $view['name'];
  }

  if (!empty($views)) {
    $form['pid'] = array(
      '#title' => t('Deployment Plan'),
      '#type' => 'select',
      '#options' => $plans,
      '#description' => t('The deployment plan to add this view to'),
    );
    $form['view_name'] = array(
      '#title' => t('View'),
      '#type' => 'select',
      '#multiple' => TRUE,
      '#required' => TRUE,
      '#options' => $views,
      '#description' => t('The view(s) to deploy'),
    );
    $form['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Add to deployment plan'),
    );

    return $form;
  }
  else {
    drupal_set_message('You have no views available to deploy');
  }
}

/**
 * Submit form callback for views_deploy_add_form().
 */
function views_deploy_add_form_submit($form, &$form_state) {
  $pid = $form_state['values']['pid'];

  foreach ($form_state['values']['view_name'] as $view_name) {
    $description = "View: $view_name";
    deploy_add_to_plan($pid, 'views', $description, $view_name, 0, DEPLOY_VIEW_GROUP_WEIGHT);
  }

  $form_state['redirect'] = "admin/build/deploy/list/$pid";
}

/**
 * Validation callback for views_deploy_add_form().
 *
 * @todo
 *   Might want to make this a little more graceful.
 */
function views_deploy_add_form_validate($form, &$form_state) {
  // Check and see if any of these views are already in the plan, if
  // so error out.
  foreach ($form_state['values']['view_name'] as $view_name) {
    $result = db_query("SELECT iid FROM {deploy_plan_items} WHERE module = 'views' AND pid = %d AND data = '%s'", $form_state['values']['pid'], $view_name);
    if (db_result($result)) {
      form_set_error('view_name', t("View $view_name already exists in the deployment plan."));
    }
  }
}
