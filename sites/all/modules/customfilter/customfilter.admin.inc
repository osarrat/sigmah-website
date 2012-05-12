<?php
// $Id: customfilter.admin.inc,v 1.1.2.48 2009/12/25 03:18:06 kiam Exp $

/**
 * @file
 * Administration page callbacks for Custom filter.
 */

/**
 * @addtogroup customfilter
 * @{
 */

/**
 * Return the export form.
 */
function customfilter_export_form(&$form_state) {
  $form = array();
  $filters = _customfilter_get_filters();
  $step = isset($form_state['storage']['step']) ? $form_state['storage']['step'] : 1;

  switch ($step) {
    case 1:
      if ($filters) {
        foreach ($filters as $filter) {
          $opt[$filter['fid']] = check_plain($filter['name']);
        }

        $form['#filters'] = $filters;

        $form['filters'] = array(
          '#type' => 'radios',
          '#title' => t('Filters'),
          '#description' => 'Choose the filter to export.',
          '#options' => $opt,
        );

        $form['submit'] = array(
          '#type' => 'submit',
          '#value' => 'Export',
        );
      }
      else {
        $form['filters'] = array(
          '#value' => '<p>' . t('There are no custom filters defined.') . '</p>',
        );
      }
      break;

    case 2:
      if ($form_state['storage']['export_data']) {
        $form['export'] = array(
          '#type' => 'textarea',
          '#title' => t('Export data'),
          '#description' => t('Copy the export text and paste it into the import form.'),
          '#cols' => 60,
          '#default_value' => $form_state['storage']['export_data'],
          '#rows' => 40,
        );
      }
      else {
        $form['export'] = array(
          '#value' => '<p>' . t('The selected filter has not been found in the database table.') . '</p>',
        );
      }
      break;
  }

  return $form;
}

function customfilter_export_form_submit($form, &$form_state) {
  $step = isset($form_state['storage']['step']) ? $form_state['storage']['step'] : 1;

  if ($step == 1) {
    $export_data = array();
    $fid = $form_state['values']['filters'];
    $filters = $form['#filters'];

    if ($filter = $filters[$fid]) {
      $export_data[] = '$filter = array(';
      $export_data[] = "  'fid' => _customfilter_map_filter_id('". $filter['fid'] ."'),";
      $export_data[] = "  'type' => base64_decode('". base64_encode($filter['type']) ."'),";
      $export_data[] = "  'name' => base64_decode('". base64_encode($filter['name']) ."'),";
      $export_data[] = "  'cache' => {$filter['cache']},";
      $export_data[] = "  'description' => base64_decode('". base64_encode($filter['description']) ."'),";
      $export_data[] = "  'shorttip' => base64_decode('". base64_encode($filter['shorttip']) ."'),";
      $export_data[] = "  'longtip' => base64_decode('". base64_encode($filter['longtip']) ."'),";
      $export_data[] = ");";
      $export_data[] = '';

      $rules = array();
      _customfilter_rules_tree($rules, $fid, 0);
      foreach ($rules as $rule) {
        $export_data[] = '$rules[] = array(';
        $export_data[] = "  'rid' => _customfilter_map_rule_id('". $rule['rid'] ."'),";
        $export_data[] = "  'fid' => _customfilter_map_filter_id('". $filter['fid'] ."'),";
        $export_data[] = "  'prid' => _customfilter_map_rule_id('". $rule['prid'] ."'),";
        $export_data[] = "  'name' => base64_decode('". base64_encode($rule['name']) ."'),";
        $export_data[] = "  'description' => base64_decode('". base64_encode($rule['description']) ."'),";
        $export_data[] = "  'enabled' => {$rule['enabled']},";
        $export_data[] = "  'matches' => base64_decode('". base64_encode($rule['matches']) ."'),";
        $export_data[] = "  'pattern' => base64_decode('". base64_encode($rule['pattern']) ."'),";
        $export_data[] = "  'replacement' => base64_decode('". base64_encode($rule['replacement']) ."'),";
        $export_data[] = "  'code' => {$rule['code']},";
        $export_data[] = "  'weight' => {$rule['weight']},";
        $export_data[] = ");";
      }
    }

    $form_state['storage']['export_data'] = count($export_data) ? implode("\n", $export_data) : '';
  }
}

/**
 * Return the form to add a new filter.
 */
function customfilter_filter_add() {
  $fid = (integer) db_result(db_query("SELECT MAX(fid) FROM {customfilter_filter}")) + 1;

  if ($fid > 128) {
    drupal_set_message(t('It is not possible to add further filters; the limit of 128 filters has been reached.'), 'error');
    return $form;
  }

  $item = array(
    'fid' => $fid,
    'type' => "filter_$fid",
    'name' => t('Filter #!fid', array('!fid' => $fid)),
    'cache' => 1,
    'description' => '',
    'shorttip' => '',
    'longtip' => '',
  );

  _customfilter_filter_add_edit_fields($form, $item);

  return $form;
}

/**
 * Validate callback for the add filter form.
 */
function customfilter_filter_add_validate($form, &$form_state) {
  if (!empty($form_state['values']['type'])) {
    $type = trim($form_state['values']['type']);

    if (!preg_match('!^[a-z0-9_]+$!', $type)) {
      form_set_error('type', t('The machine-readable name must contain only lowercase letters, numbers, and underscores.'));
    }
    elseif (db_result(db_query("SELECT 1 FROM {customfilter_filter} WHERE type = '%s'", $type))) {
      form_set_error('type', t('The machine-readable name must be unique.'));
    }
  }
}

/**
 * Submission callback for the add filter form.
 */
function customfilter_filter_add_submit($form, &$form_state) {
  drupal_write_record('customfilter_filter', $form_state['values']);
  customfilter_cache_clear('filters', $form_state['values']['fid']);

  $form_state['redirect'] = 'admin/settings/customfilter';
}

/**
 * Return the filter delete form.
 */
function customfilter_filter_delete($form_state, $fid) {
  $filters = _customfilter_get_filters();
  $form = array();

  _customfilter_breadcrumb();

  if (!$fid || !isset($filters[$fid])) {
    drupal_set_message(
      t('The filter data have not been found, or the menu callback received a wrong parameter.'),
      'error'
    );
    watchdog(
      'customfilter',
      'The filter data have not been found, or the menu callback received a wrong parameter.',
      NULL,
      WATCHDOG_ERROR
    );

    return $form;
  }

  $form['#fid'] = $fid;

  return confirm_form(
    $form, t('Are you sure you want to delete %name?', array('%name' => $filters[$fid]['name'])),
    'admin/settings/customfilter',
    NULL,
    t('Delete')
  );
}

/**
 * Submission callback for the filter delete form.
 */
function customfilter_filter_delete_submit($form, &$form_state) {
  $fid = $form['#fid'];

  db_query('DELETE FROM {customfilter_rule} WHERE fid = %d', $fid);
  db_query('DELETE FROM {customfilter_filter} WHERE fid = %d', $fid);
  customfilter_cache_clear($fid);

  $form_state['redirect'] = 'admin/settings/customfilter';
}

/**
 * Return the filter edit form.
 */
function customfilter_filter_edit($form_state, $fid) {
  $form = array();
  $filters = _customfilter_get_filters();
  $item = $filters[$fid];

  _customfilter_breadcrumb();

  if (!$fid || !isset($filters[$fid])) {
    drupal_set_message(
      t('The filter data have not been found, or the menu callback received a wrong parameter.'),
      'error'
    );
    watchdog(
      'customfilter',
      'The filter data have not been found, or the menu callback received a wrong parameter.',
      NULL,
      WATCHDOG_ERROR
    );

    return $form;
  }

  _customfilter_filter_add_edit_fields($form, $item);

  return $form;
}

/**
 * Validate callback for the filter edit form.
 */
function customfilter_filter_edit_validate($form, &$form_state) {
  if (!empty($form_state['values']['type'])) {
    $type = trim($form_state['values']['type']);

    if (!preg_match('!^[a-z0-9_]+$!', $type)) {
      form_set_error(
        'type',
        t('The machine-readable name must contain only lowercase letters, numbers, and underscores.')
      );
    }
    elseif (db_result(db_query("SELECT 1 FROM {customfilter_filter} WHERE type = '%s' AND fid <> %d", $type, $form_state['values']['fid']))) {
      form_set_error('type', t('The machine-readable name must be unique.'));
    }
  }
}

/**
 * Submission callback for the filter edit form.
 */
function customfilter_filter_edit_submit($form, &$form_state) {
  drupal_write_record('customfilter_filter', $form_state['values'], 'fid');
  customfilter_cache_clear($form_state['values']['fid']);

  $form_state['redirect'] = 'admin/settings/customfilter';
}

/**
 * Return the import form.
 */
function customfilter_import_form() {
  $form = array();

  $form['import'] = array(
    '#type' => 'textarea',
    '#title' => t('Import data'),
    '#description' => t('Paste the text you obtained from the export form.'),
    '#cols' => 60,
    '#default_value' => '',
    '#rows' => 40,
    '#required' => TRUE,
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Import'),
  );

  return $form;
}

/**
 * Validation callback for the import form.
 */
function customfilter_import_form_validate($form, &$form_state) {
  @eval($form_state['values']['import']);

  if (!isset($filter) || !isset($rules) || !is_array($filter) || !is_array($rules)) {
    form_set_error('import', t('The imported data is not valid.'));
    return;
  }

  $form_state['filter'] = $filter;
  $form_state['rules'] = $rules;
}

/**
 * Submission callback for the import form.
 */
function customfilter_import_form_submit($form, &$form_state) {
  drupal_write_record('customfilter_filter', $form_state['filter']);

  foreach ($form_state['rules'] as $rule) {
    drupal_write_record('customfilter_rule', $rule);
  }

  customfilter_cache_clear();

  drupal_set_message(
    format_plural(
      count($form_state['rules']),
      '1 replacement rule has been imported.',
      '@count replacement rules have been imported.'
    )
  );

  $form_state['redirect'] = 'admin/settings/customfilter';
}

/**
 * Return the list of replacement rules form.
 */
function customfilter_rules_form(&$form_state, $fid, $rid = 0) {
  $form = array('#tree' => TRUE);
  $rules = _customfilter_get_rules($fid, $rid);

  _customfilter_breadcrumb();

  if (count($rules)) {
    $form['fid'] = array(
      '#type' => 'hidden',
      '#value' => $fid,
    );

    $form['rules'] = _customfilter_rules_tree_form($rules, 0);

    $form['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Save'),
    );
  }

  return $form;
}

/**
 * Submission callback for the list of replacement rules form.
 */
function customfilter_rules_form_submit($form, &$form_state) {
  foreach ($form_state['values']['rules'] as $rule) {
    drupal_write_record('customfilter_rule', $rule, 'rid');
  }

  customfilter_cache_clear($form_state['values']['fid']);

  $form_state['redirect'] = "admin/settings/customfilter/{$form_state['values']['fid']}";
}

/**
 * Return the replacement rule delete form.
 */
function customfilter_rule_delete($form_state, $rid) {
  $filters = _customfilter_get_filters();
  $rule = db_fetch_object(db_query("SELECT * FROM {customfilter_rule} WHERE rid = %d", $rid));
  $rules_count = db_result(db_query("SELECT COUNT(*) FROM {customfilter_rule} WHERE prid = %d", $rid));

  if ($rule === FALSE) {
    drupal_set_message(t('The replacement rule has been deleted.'));
    drupal_goto(
      isset($_REQUEST['destination']) ? $_REQUEST['destination'] : 'admin/settings/customfilter'
    );
  }

  if (isset($filters[$rule->fid])) {
    _customfilter_breadcrumb(
      array(
        l($filters[$rule->fid]['name'], "admin/settings/customfilter/{$rule->fid}"),
      )
    );
  }

  $form = array(
    '#fid'=> $rule->fid,
    '#rid' => $rid,
  );

  if ($rules_count) {
    $msg_text .= " <p>". t('This rule has subrules. If you delete this rule, they will be deleted too.') ."</p>";
  }

  $msg_text .= t('This action cannot be undone.');

  return confirm_form(
    $form, t('Are you sure you want to delete the replacement rule %name?', array('%name' => $rule->name)),
    "admin/settings/customfilter/{$rule->fid}",
    $msg_text,
    t('Delete')
  );
}

/**
 * Submission callback for the replacement rule delete form.
 */
function customfilter_rule_delete_submit($form, &$form_state) {
  _customfilter_delete_rule($form['#rid']);
  customfilter_cache_clear($form_state['values']['fid']);

  $form_state['redirect'] = "admin/settings/customfilter/{$form['#fid']}";
}

/**
 * Return the replacement rule edit form.
 */
function customfilter_rule_edit($form_state, $op, $fid, $rid = 0) {
  $filters = _customfilter_get_filters();
  $form = array();

  if (isset($filters[$fid])) {
    _customfilter_breadcrumb(
      array(
        l($filters[$fid]['name'], "admin/settings/customfilter/{$fid}"),
      )
    );
  }

  if ($op == 'edit') {
    $item = db_fetch_array(db_query("SELECT * FROM {customfilter_rule} WHERE rid = %d", $rid));
  }
  elseif ($op == 'add') {
    $item = array(
      'rid' => (integer) db_result(db_query("SELECT MAX(rid) FROM {customfilter_rule}")) + 1,
      'prid' => $rid,
      'fid' => $fid,
      'name' => '$1',
      'description' => '',
      'enabled' => 1,
      'matches' => 1,
      'pattern' => '/regex/i',
      'replacement' => 'Regular Expressions',
      'code' => 0,
      'weight' => 0,
    );
  }

  $matchopt = drupal_map_assoc(range(0, 99));

  $form['rid'] = array(
    '#type' => 'value',
    '#value' => $item['rid'],
  );

  $form['fid'] = array(
    '#type' => 'value',
    '#value' => $fid,
  );

  $form['prid'] = array(
    '#type' => 'value',
    '#value' => $item['prid'],
  );

  $form['operation'] = array(
    '#type' => 'value',
    '#value' => $op,
  );

  if ($item['prid'] != 0) {
    $form['matches'] = array(
      '#type' => 'select',
      '#title' => t('# Match'),
      '#description' => t('n-th matched substring in parent rule. This replacement rule will replace only for that substring.'),
      '#options' => $matchopt,
      '#default_value' => $item['matches'],
    );
  }

  $form['name'] = array(
    '#type' => 'textfield',
    '#description' => t('The name of this replacement rule.'),
    '#title' => t('Name'),
    '#default_value' => $item['name'],
    '#required' => TRUE,
  );

  $form['weight'] = array(
    '#title' => t('Weight'),
    '#default_value' => $item['weight'],
  );

  $form['enabled'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enabled'),
    '#description' => t('If selected, the rule is used.'),
    '#default_value' => $item['enabled'],
  );

  $form['description'] = array(
    '#type' => 'textarea',
    '#title' => t('Description'),
    '#description' => t('The description of this rule.'),
    '#default_value' => $item['description'],
  );

  $form['pattern'] = array(
    '#type' => 'textarea',
    '#title' => t('Pattern'),
    '#description' => t('The regular expression to use. Look at <a href="http://www.php.net/manual/en/regexp.reference.php">Regular Expression Details</a> for more help.'),
    '#default_value' => $item['pattern'],
    '#rows' => 3,
  );

  $form['code'] = array(
    '#type' => 'checkbox',
    '#title' => t('PHP Code'),
    '#description' => t('If selected, the replacement text is PHP code.'),
    '#default_value' => $item['code'],
  );

  $form['replacement']['replacement'] = array(
    '#type' => 'textarea',
    '#title' => t('Replacement text'),
    '#description' => t('The replacement text that will replace the matched string. Use $n (i.e. $1, $25) or ${n} (i.e. ${1}, ${25}), with n range from 0 to 99, to get the n-th original strings matched ($0 represents the entire matched string). If you select <strong>PHP Code</strong>, you can enter PHP code that will be executed during the elaboration of the rules. n-th matched string is provided in <code>$matches[n]</code>, and there is a global variable <code>$vars</code> you can use to store values that will be kept during the execution of different rules of the same filter. PHP code must set a value for <code>$result</code>, and must not be entered between <code><</code><code>?php ?></code>. Note that executing incorrect PHP-code can break your Drupal site.'),
    '#default_value' => $item['replacement'],
    '#rows' => 16,
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );

  return $form;
}

/**
 * Submission callback for the replacement rule edit form.
 */
function customfilter_rule_edit_submit($form, &$form_state) {
  switch ($form_state['values']['operation']) {
    case 'edit':
      drupal_write_record('customfilter_rule', $form_state['values'], 'rid');
      break;

    case 'add':
      drupal_write_record('customfilter_rule', $form_state['values']);
      break;
  }

  customfilter_cache_clear($form_state['values']['fid']);

  $form_state['redirect'] = "admin/settings/customfilter/{$form_state['values']['fid']}";
}

/**
 * Module settings page.
 */
function customfilter_settings() {
  $rows = array();

  foreach (_customfilter_get_filters() as $id => $filter) {
    $row = array();

    $row[] = l($filter['name'], "admin/settings/customfilter/{$id}");
    $row[] = l(t('edit'), "admin/settings/customfilter/{$id}/edit");
    $row[] = l(t('delete'), "admin/settings/customfilter/{$id}/delete");

    $rows[] = $row;
  }

  if (!count($rows)) {
    $rows[] = array(
      array(
        'data' => t('No filters have been defined.'),
        'colspan' => 3,
      ),
    );
  }

  $header[] = t('Name');
  $header[] = array(
    'data' => t('Operations'),
    'colspan' => '2'
  );

  return theme('table', $header, $rows);
}

/**
 * Theme for draggable tree of replacement rules.
 */
function theme_customfilter_rules_form($form) {
  $rows = array();

  if (isset($form['rules'])) {
    foreach (element_children($form['rules']) as $rid) {
      $row = array();

      $element = &$form['rules'][$rid];

      $element['prid']['#attributes']['class'] = 'rule-prid';
      $element['rid']['#attributes']['class'] = 'rule-rid';
      $element['weight']['#attributes']['class'] = 'rule-weight';

      $row[] = (
        theme('indentation', $element['level']['#value']) .
        drupal_render($element['name'])
      );
      $row[] = (
        drupal_render($element['rid']) .
        drupal_render($element['prid']) .
        drupal_render($element['weight'])
      );
      $row[] = drupal_render($element['enabled']);
      $row[] = drupal_render($element['operations']['add']);
      $row[] = drupal_render($element['operations']['edit']);
      $row[] = drupal_render($element['operations']['delete']);

      $rows[] = array(
        'data' => $row,
        'class' => 'draggable',
      );
    }
  }

  if (!count($rows)) {
    $rows[] = array(
      array(
        'data' => t('There are no replacement rules currently defined.'),
        'colspan' => '6',
      )
    );
  }

  $header[] = t('Name');
  $header[] = t('Weight');
  $header[] = t('Enabled');
  $header[] = array(
    'data' => t('Operations'),
    'colspan' => '3',
  );

  drupal_add_tabledrag('customfilter-rules-tree', 'order', 'sibling', 'rule-weight', NULL, NULL, TRUE);
  drupal_add_tabledrag('customfilter-rules-tree', 'match', 'parent', 'rule-prid', 'rule-prid', 'rule-rid', TRUE);

  return theme('table', $header, $rows, array('id' => 'customfilter-rules-tree')) .
    drupal_render($form);
}

function _customfilter_filter_add_edit_fields(&$form, $item) {
  $form['fid'] = array(
    '#type' => 'value',
    '#value' => $item['fid'],
  );

  $form['type'] = array(
    '#type' => 'textfield',
    '#title' => t('Type'),
    '#description' => t('The machine-readable name of the filter. This name must contain only lowercase letters, numbers, and underscores.'),
    '#default_value' => $item['type'],
    '#maxlength' => 32,
    '#element_validate' => array('customfilter_filter_type_validate'),
    '#required' => TRUE,
  );

  $form['name'] = array(
    '#type' => 'textfield',
    '#title' => t('Name'),
    '#description' => t('The human-readable name of the filter. It is recommended that this name begin with a capital letter and contain only letters, numbers, and <em>spaces</em>.'),
    '#default_value' => $item['name'],
    '#maxlength' => 64,
    '#required' => TRUE,
  );

  $form['cache'] = array(
    '#type' => 'checkbox',
    '#title' => t('Cache'),
    '#description' => t('If checked, the content will be cached.'),
    '#default_value' => $item['cache'],
  );

  $form['description'] = array(
    '#type' => 'textarea',
    '#title' => t('Description'),
    '#description' => t('A description of the purpose of this filter.'),
    '#default_value' => $item['description'],
  );

  $form['shorttip'] = array(
    '#type' => 'textarea',
    '#title' => t('Tips (short)'),
    '#default_value' => $item['shorttip'],
    '#rows' => 5,
  );

  $form['longtip'] = array(
    '#type' => 'textarea',
    '#title' => t('Tips (full)'),
    '#default_value' => $item['longtip'],
    '#rows' => 20,
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );
}

/**
 * Map the imported filter ID to a new one.
 */
function _customfilter_map_filter_id($id) {
  static $base = 0, $fids = array();

  if (!$base) {
    $base = (integer) db_result(db_query("SELECT MAX(fid) FROM {customfilter_filter}")) + 1;
  }

  if (!isset($fids[$id])) {
    $fids[$id] = $base++;
  }

  return $fids[$id];
}

/**
 * Map the imported rule ID to a new one.
 */
function _customfilter_map_rule_id($id) {
  static $base = 0, $rids = array();

  if (!$id) {
    return 0;
  }

  if (!$base) {
    $base = (integer) db_result(db_query("SELECT MAX(rid) FROM {customfilter_rule}")) + 1;
  }

  if (!isset($rids[$id])) {
    $rids[$id] = $base++;
  }

  return $rids[$id];
}

/**
 * Helper function for customfilter_export_form.
 * Return the list made of a rule, and all the sub-rules.
 */
function _customfilter_rules_tree(&$rules, $fid, $prid = 0) {
  $result = db_query("SELECT * FROM {customfilter_rule} WHERE fid = %d and prid = %d ORDER BY weight", $fid, $prid);

  while ($rule = db_fetch_array($result)) {
    $rules[] = $rule;
    _customfilter_rules_tree($rules, $fid, $rule['rid']);
  }
}

/**
 * Return the rules tree form.
 */
function _customfilter_rules_tree_form($rules, $level = 0) {
  $base = 'admin/settings/customfilter';
  $form = array();

  foreach ($rules as $rule) {
    $fid = $rule['fid'];
    $rid = $rule['rid'];

    $form[$rid] = array(
      'name' => array(
        '#value' => l($rule['name'], "$base/$fid/$rid/edit"),
      ),
      'enabled' => array(
        '#type' => 'checkbox',
        '#default_value' => $rule['enabled'],
      ),
      'weight' => array(
        '#type' => 'weight',
        '#default_value' => $rule['weight'],
      ),
      'rid' => array(
        '#type' => 'hidden',
        '#value' => $rid,
      ),
      'prid' => array(
        '#type' => 'textfield',
        '#default_value' => $rule['prid'],
        '#size' => 4,
      ),
      'level' => array(
        '#type' => 'value',
        '#value' => $level,
      ),
    );

    $form[$rid]['operations'] = array(
      'add' => array(
        '#value' => l(t('add'), "$base/$fid/$rid/add"),
      ),
      'edit' => array(
        '#value' => l(t('edit'), "$base/$fid/$rid/edit"),
      ),
      'delete' => array(
        '#value' => l(t('delete'), "$base/$fid/$rid/delete"),
      ),
    );

    if ($rule['sub']) {
      $form += _customfilter_rules_tree_form($rule['sub'], $level + 1);
    }
  }

  return $form;
}

/**
 * @} End of "addtogroup customfilter".
 */