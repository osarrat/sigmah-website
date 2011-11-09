<?php
// $Id: tac_fields.admin.inc,v 1.1.2.1 2010/08/07 02:12:04 xjm Exp $

/**
 * @file
 * Administrative interface for TAC Fields.
 */


/**
 * Menu callback for TAC Fields admin page (admin/user/tac_fields).
 */
function tac_fields_admin($op=NULL, $field=NULL, $rid=NULL, $arg=NULL) {
  drupal_set_message("Warning: This module is experimental.  Do not use on a production site.", 'warning', FALSE);

  $roles = _taxonomy_access_user_roles();
  $controlled_fields = _tac_fields_controlled_fields();
  if ($field && in_array($field, $controlled_fields)) {
    if (is_numeric($rid) AND isset($roles[$rid])) {
      switch ($op) {
        case 'edit':
          return drupal_get_form('tac_fields_admin_form', $field, $rid);
        case 'delete':
          return drupal_get_form('tac_fields_delete', $field, $rid);
      }
    }
    elseif (($op == 'delete') AND !isset($rid)) {
      return drupal_get_form('tac_fields_delete', $field);
    }
  }
  elseif (!isset($op) AND !isset($rid)) {
    return theme_tac_fields_admin();
  }

  // If we get here, page arguments were invalid.
  return drupal_not_found();
}

/**
 * Renders the main admin page (at admin/user/tac_fields).
 *
 * @return
 *     String containing rendered markup for the field's administration.
 */
function theme_tac_fields_admin() {

  $output = '';

  $add_form = tac_fields_add_form();

  $output .= drupal_get_form('tac_fields_add_form');

  // Get a list of all fields we control.
  $fields = _tac_fields_controlled_fields();

  foreach ($fields as $field) {
    $output .= _tac_fields_admin_for_field($field);
  }
  return $output;
}


/**
 * Renders admin table for each field, with links to configure each role.
 * 
 * @param $field
 *     The machine name of the content field.
 *
 * @return
 *     String of HTML markup containing a header and table.
 *
 * @todo
 *     Return empty string if the field is not a controlled field?
 */
function _tac_fields_admin_for_field($field) {
  $output = '';

  $output .=     
    "<h2>" 
    . t("Configuration for %field", array('%field' => $field))
    . "</h2>" 
    . t('To delete all access rules below and return this field to its default behavior, you can <a href="@path">release control of %field</a>.', 
      array('%field' => $field,
        '@path' => url("admin/user/tac_fields/delete/$field")));

  $roles = _taxonomy_access_user_roles();

  // Render role/permission overview:
  $header = array(t('Role'), array('data' => '&nbsp;'));
  $rows = array();
  
  $result = db_query(
    "SELECT rid FROM {term_field_access_defaults} WHERE vid=0 AND field='%s'",
    $field);
  $active = array();
  while ($role = db_fetch_array($result)) {
    $active[$role['rid']] = TRUE;
  }
  foreach ($roles as $rid => $name) {
    $ops = array();
    if (!empty($active[$rid])) {
      //only allow delete for "extra" roles
      if ($rid > 2) {
        $ops[] = 
          l(t("disable"), 
            "admin/user/tac_fields/delete/$field/$rid");
      }
      $ops[] = 
        l(t("edit"), 
          "admin/user/tac_fields/edit/$field/$rid");
    }
    else {
      $ops = array(
        l(t("enable"), 
          "admin/user/tac_fields/edit/$field/$rid"));
    }
    $rows[] = array($name, array('data' => implode(' | ', $ops), 'align' => 'right'));
  }

  $output .= theme('table', $header, $rows);

  return $output;
}


/**
 * Form to choose a field to control with TAC.
 *
 * @todo
 *     Group select options by content type instead of field type?
 *
 * @return
 *     Form object.
 */
function tac_fields_add_form() {
  $options = array();
  $form = array();

  $fields = content_fields();
  $controlled_fields = _tac_fields_controlled_fields();

  // Assemble a list of all CCK Fields, minus fields we already control.
  foreach ($fields as $field) {
    if (!in_array($field['field_name'], $controlled_fields)) {
      $options[$field['type']][$field['field_name']] = 
        t($field['widget']['label']) .' ('. $field['field_name'] .')';
    }
  }

  if (!empty($options)) {
    $form['add'] = array(
      '#type' => 'select',
      '#title' => t('Add field'),
      '#required' => TRUE,
      '#description' => t('Choose a field to control with TAC Fields.'),
      '#options' => $options,
    );
    $form['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Add'),
    );
  }
  else {
    $form['no_choices'] = array(
      '#value' => t('There are no fields available that are not already controlled by TAC Fields.  See below to configure any fields that are already controlled, or add fields to one of your <a href="@content_types">content types</a>.',
         array('@content_types' => url("admin/content/types"))),
    );
  }

  return $form;
}


/**
 * Submit handler for tac_fields_add_form().
 */
function tac_fields_add_form_submit($form, &$form_state) {
  $values = $form_state['values'];
  if ($field = $values['add']) {

    // Check to see whether it's a valid CCK field and not already controlled.
    $fields = content_fields();
    $controlled_fields = _tac_fields_controlled_fields();

    if (in_array($field, array_keys($fields)) 
      && !in_array($field, $controlled_fields)) {

      // Add global defaults for anonymous & authenticated users to the db.
      $row = new stdClass();
      $row->vid = 0;
      $row->field = $field;
      
      for ($i=1; $i<=2; $i++) {
        $row->rid = $i;
        drupal_write_record('term_field_access_defaults', $row);
      }
    }
  }
}


function tac_fields_delete($form, $field, $rid=NULL) {
  $controlled_fields = _tac_fields_controlled_fields();
  if (in_array($field, $controlled_fields)) {
    if (isset($_POST['confirm'])) {
      if (is_numeric($rid) && ($rid > 2)) {
        return _tac_fields_disable_role($field, $rid);
      }
      elseif (is_null($rid)) {
        return _tac_fields_disable_field($field);
      }
    }
    else {
      if (is_numeric($rid) && ($rid > 2)) {
        $message = 
          t("Are you sure you wish to delete all of the %field field's access rules for role %rid?", 
            array('%field' => $field, '%rid' => $rid));
      }
      elseif (is_null($rid)) {
        $message = 
          t("Are you sure you wish to release control of the %field field and delete <strong>all</strong> of its access rules?", 
            array('%field' => $field, '%rid' => $rid));
      }
      else {
        return drupal_not_found();
      }
      return confirm_form($form, $message,
        'admin/user/tac_fields',
        t('This action cannot be undone.'),
        t('Delete all'),
        t('Cancel'));
    }
  }

  return drupal_not_found();
}

/**
 * Helper function to delete all access rules for a field/row pair.
 *
 * @param $field
 *     The machine name of the field.
 * @param $rid
 *     The role id for which to remove access rules for the field.
 */
function _tac_fields_disable_role($field, $rid) {
  // Remove all records for the role/field pair from the database.
  db_query(
    "DELETE FROM {term_field_access} 
     WHERE field = '%s' AND rid = %d", $field, $rid);
  db_query(
    "DELETE FROM {term_field_access_defaults} 
     WHERE field = '%s' AND rid = %d", $field, $rid);

  drupal_set_message(t('All term access rules for field %field and role %rid have been deleted.', array('%field' => $field, '%rid' => $rid)));
  drupal_goto('admin/user/tac_fields');
}


/**
 * Helper function to delete all access rules for a field.
 *
 * @param $field
 *     The machine name of the field we are releasing.
 */
function _tac_fields_disable_field($field) {
  // Remove all records for the field from the database.
  db_query(
    "DELETE FROM {term_field_access} 
     WHERE field = '%s'", $field);
  db_query(
    "DELETE FROM {term_field_access_defaults} 
     WHERE field = '%s'", $field);

  drupal_set_message(t('All term access rules for field %field have been deleted.', array('%field' => $field)));
  drupal_goto('admin/user/tac_fields');
}


/**
 * Main administration form, with grid of permissions per term.
 *
 * @param $field
 *     The field to configure.
 * @param $rid
 *     The role to configure.
 */
function tac_fields_admin_form($form_state, $field, $rid) {
  // Fetch all default grants
  $r = db_query(
    "SELECT * FROM {term_field_access_defaults} 
     WHERE field = '%s' AND rid = %d", 
    $field, $rid);
  while ($row = db_fetch_array($r)) {
    $default_grants[$row['vid']] = $row;
  }

  // If we are adding a role, no global default is set yet, so insert it now.
  if (empty($default_grants[0]) && isset($rid) && isset($field)) {
    // Assemble a $row object for Schema API.
    $row = new stdClass();
    $row->vid = 0;
    $row->field = $field;
    $row->rid = $rid;
    $row->grant_view = 1;
    $row->grant_update = 0;

    // Insert the default.
    drupal_write_record('term_field_access_defaults', $row);
  }

  // Fetch all grants
  $result = db_query(
    "SELECT * FROM {term_field_access} 
     WHERE field = '%s' AND rid = %d", 
    $field, $rid);
  while ($row = db_fetch_array($result)) {
    $grants[$row['tid']] = $row;
  }

    $form['instructions'] = array(
    '#value' => _tac_fields_admin_instructions_html(),
    '#weight' => '20',
  );
  $form['rid'] = array('#type' => 'value', '#value' => $rid);
  $form['field'] = array('#type' => 'value', '#value' => $field);
  $form['grants'] = $form['selected_terms'] = $form['selected_defaults'] = array('#tree' => TRUE);
  
  // Global default
  $global_defaults = empty($default_grants[0]) ? NULL : $default_grants[0];
  $form['vocabs'][0]['#title'] = 'Global';
  $form['grants'][0][0] = tac_fields_build_row($global_defaults);
  $form['selected_defaults'][0] = array(
     '#type' => 'checkbox',
     '#disabled' => TRUE,
     '#title' => t('<em>default</em>'),
     '#description' => t("can't be disabled without disabling this role or field"),
  );

  foreach (taxonomy_get_vocabularies() as $vid => $vocabulary) {
    $form['vocabs'][$vid]['#title'] = check_plain($vocabulary->name);
    if (isset($default_grants[$vid])) {
      $form['grants'][$vid][0] = tac_fields_build_row($default_grants[$vid]);
      $form['selected_defaults'][$vid] = array(
         '#type' => 'checkbox',
         '#title' => t('<em>default</em>'),
      );
    }
    else {
      $add_items[$vocabulary->name]["default $vid"] = t('*default*');
    }

    if ($tree = taxonomy_get_tree($vid)) {
      foreach ($tree as $term) {
        if (isset($grants[$term->tid])) {
          $form['grants'][$vid][$term->tid] = tac_fields_build_row($grants[$term->tid]);
          $form['selected_terms'][$term->tid] = array(
             '#type' => 'checkbox',
             '#title' => str_repeat('&nbsp;&nbsp;', $term->depth) . check_plain($term->name),
          );
        }
        else {
          $add_items[$vocabulary->name]["term $term->tid"] = str_repeat('-', $term->depth) . check_plain($term->name);
        }
      }
    }
  }
  //New grant row
  if (isset($add_items)) {
    $form['new']['grants'] = tac_fields_build_row();
    $form['new']['#tree'] = TRUE;
    $form['new']['item'] = array(
      '#type' => 'select',
      '#options' => $add_items,
    );
    $form['new']['recursive'] = array(
      '#type' => 'checkbox',
      '#title' => t('with children'),
      '#description' => t('Add child terms recursively with these values.'),
    );
    $form['new']['add'] = array(
      '#type' => 'submit',
      '#value' => t('Add'),
    );
  }
  
  $form['delete'] = array(
    '#type' => 'submit',
    '#value' => t('Delete selected'),
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save all'),
  );

  return $form;
}


/**
 * Assembles an admin form row with the appropriate radio buttons.
 *
 * @param $grants
 *     Default grants to set, if any.
 *
 * @return
 *     Form array for the row.
 */
function tac_fields_build_row($grants = NULL) {
  $form['#title'] = '';
  $form['#tree'] = TRUE;
  foreach (array('view', 'update') as $grant) {
    $form[$grant] = array(
      '#type' => 'radios', 
      '#options' => array('1' => '', '0' => '', '2' => ''), //1: Allow, 0: Ignore, 2: Deny
      '#default_value' => is_string($grants['grant_'. $grant]) ? $grants['grant_'. $grant] : '0',
      '#required' => TRUE,
     );
  }
  return $form;
}


/**
 * Renders the admin form in an HTML table.
 */
function theme_tac_fields_admin_form($form) {
  $roles = _taxonomy_access_user_roles();
  $header = array(
    array( 'data' => t('Category'), 'colspan' => 3),
    array( 'data' => t('View'), 'colspan' => 4),
    array( 'data' => t('Update'), 'colspan' => 4),
  );
  $sub_header = array(
    '&nbsp;<strong>'. t('<acronym title="Allow">A</acronym>') .'</strong>',
    '&nbsp;<strong>'. t('<acronym title="Ignore">I</acronym>') .'</strong>',
    '&nbsp;<strong>'. t('<acronym title="Deny">D</acronym>') .'</strong>',
    '&nbsp;',
  );
  $sub_header = array_merge(array('&nbsp;'), $sub_header, $sub_header);
  $node_grant_types = array('view', 'update');

  $radios = array('1' => t('Allow'), '0' => t('Ignore'), '2' => t('Deny'));

  drupal_set_title(t('%field: Grants for %role', array('%field' => $form['field']['#value'], '%role' => $roles[$form['rid']['#value']])));

  $rows = array();

  foreach (array_keys($form['vocabs']) as $vid) {
    if (is_numeric($vid) AND isset($form['grants'][$vid])) {
      $row = $sub_header;
      $row[0] = array('data' => '<h3>'. check_plain($form['vocabs'][$vid]['#title']) .'</h3>', 'colspan' => 3);
      $rows[] = $row;
      foreach (array_keys($form['grants'][$vid]) as $tid) {
        if (is_numeric($tid)) {
          $select_key = $tid? 'selected_terms' : 'selected_defaults';
          $select_id = $tid? $tid : $vid;
          $row = array(
            array('data' => drupal_render($form[$select_key][$select_id]), 'colspan' => 3),
          );
          foreach ($node_grant_types as $grant) {
            foreach (array_keys($radios) as $key) {
              // I need this hack to display radio buttons horizontally (instead of standard form 'radios')
              $row[] = array('data' => drupal_render($form['grants'][$vid][$tid][$grant][$key]));
            }
            $row[] = '&nbsp;';
          }
          $rows[] = $row; 
        }
      }
    }
  }
  if (isset($form['new'])) {
    $row = $sub_header;
    $row[0] = array('data' => '<h3>'. t('New') .'</h3>', 'colspan' => 3);
    $rows[] = $row;

    $row = array(
      array(
        'data' => drupal_render($form['new']['item'])
        . drupal_render($form['new']['recursive']), 
        'colspan' => '2',
      ),
      drupal_render($form['new']['add']),
    );
    foreach ($node_grant_types as $grant) {
      foreach (array_keys($radios) as $key) {
        // I need this hack to display radio buttons horizontally (instead of standard form 'radios')
        $row[] = array('data' => drupal_render($form['new']['grants'][$grant][$key]));
      }
      $row[] = '&nbsp;';
    }
    $rows[] = $row; 
    $row = array();
  }


  $output = '';
  $output .= theme('table', $header, $rows);
  $output .= drupal_render($form);

  return $output;

}


/**
 * Submit handler for permission administration form.
 *
 * @todo
 *     This is cloned from TAC; finish.
 */
function tac_fields_admin_form_submit($form, &$form_state) {
  $values = $form_state['values'];
  $field = $values['field'];
  $rid = $values['rid'];

  switch ($values['op']) {

    case t('Delete selected'):
      if (is_array($values['selected_terms'])) {
        foreach ($values['selected_terms'] as $tid => $enabled) {
          if ($enabled) {
             db_query(
              "DELETE FROM {term_field_access} 
               WHERE field = '%s' AND rid = %d AND tid = %d", 
              $field, $rid, $tid
            );
           }
        }
      }
      if (is_array($values['selected_defaults'])) {
        foreach ($values['selected_defaults'] as $vid => $enabled) {
          if ($enabled) {
             db_query(
              "DELETE FROM {term_field_access_defaults} 
               WHERE field = '%s'  AND rid = %d AND vid = %d",
              $field, $rid, $vid
            );
           }
        }
      }
      break;

    case t('Add'):
      $new = $values['new'];
      list($type, $id) = explode(' ', $new['item']);
      if ($type == 'term') {
        if ($new['recursive'] == 1) {
          tac_fields_recursive_update($id, $field, $rid, $new['grants']);
        } 
        else {
          tac_fields_update_grants($id, $field, $rid, $new['grants']);
        }
      }
      elseif ($type == 'default') {
        tac_fields_update_defaults($id, $field, $rid, $new['grants']);
      }
      break;

    case t('Save all'):
      foreach ($values['grants'] as $vid => $rows) {
        foreach ($rows as $tid => $grants) {

          // Check the default values for this row.
          $defaults = array();
          foreach ($grants as $grant_name => $value) {
            $defaults[$grant_name] = 
              $form['grants'][$vid][$tid][$grant_name]['#default_value'];
          }

          // Proceed if the user changed the row (values differ from defaults).
          if ($defaults != $grants) {
            if ($tid == 0) {
              tac_fields_update_defaults($vid, $field, $rid, $grants);
            }
            else {
              tac_fields_update_grants($tid, $field, $rid, $grants);
            }
          }
        }
      }

      drupal_goto('admin/user/tac_fields');
      break;
  }
}

function _tac_fields_admin_instructions_html() {
  return "hi";
}

function tac_fields_update_grants($tid, $field, $rid, $grants) {
  // Assemble a $row object for Schema API.
  $row = new stdClass();
  $row->tid = $tid;
  $row->field = $field;
  $row->rid = $rid;

  if (isset($grants) && is_array($grants)) {
    foreach ($grants as $op => $value) {
      if (is_numeric($value)) {
        $grant_name = "grant_$op";
        $row->$grant_name = $value;
      }
    }
  }

  // Delete old entry.
  db_query(
    "DELETE FROM {term_field_access} 
     WHERE field = '%s' AND tid = %d AND rid = %d", $field, $tid, $rid);

  // Insert new entry.
  drupal_write_record('term_field_access', $row);
}

function tac_fields_update_defaults($vid, $field, $rid, $grants) {
  // Assemble a $row object for Schema API.
  $row = new stdClass();
  $row->vid = $vid;
  $row->field = $field;
  $row->rid = $rid;

  if (isset($grants) && is_array($grants)) {
    foreach ($grants as $op => $value) {
      if (is_numeric($value)) {
        $grant_name = "grant_$op";
        $row->$grant_name = $value;
      }
    }
  }

  // Delete old entry.
  db_query(
    "DELETE FROM {term_field_access_defaults} 
     WHERE field = '%s' AND vid = %d AND rid = %d", $field, $vid, $rid);

  // Insert new entry.
  drupal_write_record('term_field_access_defaults', $row);
}

function tac_fields_recursive_update($tid, $field, $rid, $grants) {
  // First, process the original.
  tac_fields_update_grants($tid, $field, $rid, $grants);
  
  // Process the children.
  $ran_tids = array(); // tids that have been processed.
  $run_tids = array($tid); // tids that are in the queue to be processed.
  while (count($run_tids) > 0) { 
    foreach ($run_tids as $run_key => $run_tid) {
      // Some basic loop protection.
      if (!(array_search($run_tid, $ran_tids) === FALSE)) {
        drupal_set_message(
          t("Loop detected for tid %run_tid. Stopping.",
            array('%run_tid' => $run_tid)
           ));
        $run_tids = array(); // stop the execution
      } 
      else {
        $result = db_query('SELECT th.tid FROM {term_hierarchy} th WHERE th.parent = %d', $run_tid);
        // If this tid has children, update grants and queue the children
        while ($row = db_fetch_array($result)) {
          tac_fields_update_grants($row['tid'], $field, $rid, $grants);
          $run_tids[] = $row['tid'];
        }

        // Remove this tid from the queue and mark as processed,
        unset($run_tids[$run_key]);
        $ran_tids[] = $run_tid;
      }
    }
  }
}