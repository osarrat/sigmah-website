<?php
/**
 * Display vocabulary deploy form.
 *
 * @param $form_state
 *   FAPI form state
 * @param $vid
 *   The vid of the vocabulary we want to deploy.
 * @return
 *   FAPI array
 * @ingroup forms
 * @see taxonomy_vocabulary_deploy_add_form_submit()
 */
function taxonomy_vocabulary_deploy_add_form($form_state, $vid) {
  // Get options list for vocabularies
  $vocabularies = taxonomy_get_vocabularies();
  $options = array();
  foreach ($vocabularies as $vid => $vocabulary) {
    $options[$vid] = $vocabulary->name;
  }

  $form = deploy_get_server_form();

  $form['vid'] = array(
    '#title' => t('Vocabulary'),
    '#type' => 'select',
    '#options' => $options,
    '#required' => TRUE,
    '#description' => t('The vocabulary you want to deploy'),
    '#weight' => 0,
  );

  return $form;
}

/**
 * Submit handler for taxonomy_vocabulary_deploy_add_form().
 */
function taxonomy_vocabulary_deploy_add_form_submit($form, &$form_state) {
  $sid = $form_state['values']['sid'];
  $vid = $form_state['values']['vid'];
  
  // If there is already aplan for this vocabulary, then empty it and use it.
  // Otherwise create one.
  $pid = deploy_plan_exists('Vocabulary '. $vid);
  if (!$pid) {
    $pid = deploy_create_plan('Vocabulary '. $vid, 'Internal plan to deploy a specific vocabulary', 1);
  }
  else {
    deploy_empty_plan($pid);
  }

  if (deploy_plan_init($pid, $sid, $form_state['values'])) {
    $result = db_query("SELECT tid, vid, name FROM {term_data} WHERE vid = %d", $vid);
    while ($term = db_fetch_array($result)) {
      deploy_add_to_plan($pid, 'taxonomy_term', $term['name'], $term['tid'], 0, DEPLOY_TAXONOMY_TERM_GROUP_WEIGHT);
    }
  
    $vocabulary = taxonomy_vocabulary_load($vid);
    deploy_add_to_plan($pid, 'taxonomy_vocabulary', $vocabulary->name, $vid, 0, DEPLOY_TAXONOMY_VOCABULARY_GROUP_WEIGHT);
  
    taxonomy_deploy_deploy_check_cleanup($pid);
    
    $form_state['redirect'] = "admin/build/deploy/deploy_push_batch/$pid";
  }
  else {
    $dlid = variable_get('deploy_log_id', '');
    deploy_plan_cleanup();

    // Redirect to the log overview page for this push.
    $form_state['redirect'] = "admin/build/deploy/logs/details/$dlid";
  }
}