<?php
// $Id: taxonomy_access_admin.inc,v 1.15.2.20 2010/03/15 17:11:19 xjm Exp $

/**
 * @file
 * Administrative interface for taxonomy access control.
 */

/**
 * Cache a list of all roles.
 *
 * @return
 *    An array of roles from user_roles().
 */
function _taxonomy_access_user_roles() {
  static $roles;
  if (!is_array($roles)) {
    $roles = user_roles();
  }
  return $roles;
}

/**
 * Menu callback (for admin/user/taxonomy_access).
 * Renders the TAC permissions administration form.
 *
 * @param $op
 *     The operation to perform, if any.
 *     Options are 'edit' or 'delete' (access rules for the role).
 * @param $rid
 *     The role id of the role to configure.
 * @param $arg
 *     Additional arguments.  (Not used.)
 *
 * @return
 *     Form to render.
 */
function taxonomy_access_admin($op = NULL, $rid = NULL, $arg = NULL) {
  $roles = _taxonomy_access_user_roles();
  if (is_numeric($rid) AND isset($roles[$rid])) {
    switch ($op) {
    case 'edit':
      return drupal_get_form('taxonomy_access_admin_form', $rid);
    case 'delete':
      return drupal_get_form('taxonomy_access_admin_delete_role', $rid);
    }
  }
  elseif (!isset($op) AND !isset($rid)) {
    return theme_taxonomy_access_admin();
  }
  /**
   * @todo
   *     Something odd happens here.
   */
  else return drupal_not_found();
}

/**
 * Renders the main administration page, with links to configure each role.
 *
 * @return
 *     Themed table with one row for each role.
 */
function theme_taxonomy_access_admin() {
  $roles = _taxonomy_access_user_roles();

  // Render role/permission overview:
  $header = array(t('Role'), array('data' => '&nbsp;'));
  $rows = array();

  $result = db_query('SELECT rid FROM {term_access_defaults} WHERE vid=0 ');
  $active = array();
  while ($role = db_fetch_array($result)) {
    $active[$role['rid']] = TRUE;
  }
  foreach ($roles as $rid => $name) {
    $ops = array();
    if (!empty($active[$rid])) {
      //only allow delete for "extra" roles
      if ($rid > 2) {
        $ops[] = l(t("disable"), "admin/user/taxonomy_access/delete/$rid");
      }
      $ops[] = l(t("edit"), "admin/user/taxonomy_access/edit/$rid");
    }
    else {
      $ops = array(l(t("enable"), "admin/user/taxonomy_access/edit/$rid"));
    }
    $rows[] = array($name, array('data' => implode(' | ', $ops), 'align' => 'right'));
  }

  return theme('table', $header, $rows);
}

/**
 * Form callback to delete all access rules for a particular role.
 * @param $rid
 *     The role id to disable.
 */
function taxonomy_access_admin_delete_role($form_state, $rid) {
  if (is_numeric($rid) AND $rid > 2) {
    $r = db_query(
      'SELECT grant_view, grant_update, grant_delete
       FROM {term_access_defaults} 
       WHERE vid=0 AND rid=%d', $rid);
    $role_global_default = db_fetch_array($r);
    if ($role_global_default) {
      if ($_POST['confirm']) {

        // Compare this role's global default node access to auth user role.
        $auth_global_default = db_fetch_array(db_query(
          'SELECT grant_view, grant_update, grant_delete
           FROM {term_access_defaults} 
           WHERE vid=0 AND rid=2'));

        if ($role_global_default == $auth_global_default) {
          $affected_nodes = _taxonomy_access_get_nodes_for_role($rid);
        }
        else {
          // If the global default is different, this role's access changes on
          // all uncontrolled nodes, so flag node access for rebuild.
          node_access_needs_rebuild(TRUE);
        }
        db_query('DELETE FROM {term_access} WHERE rid=%d', $rid);
        db_query('DELETE FROM {term_access_defaults} WHERE rid=%d', $rid);
        if (!empty($affected_nodes)) {
          _taxonomy_access_node_access_update($affected_nodes);
        }

        drupal_set_message(t('All term access rules deleted for role %rid.',
            array('%rid' => $rid)));
        drupal_goto('admin/user/taxonomy_access');
      }

      else {
        return confirm_form($form,
          t('Are you sure you want to delete all grant rules for role %rid?',
            array('%rid' => $rid)),
          'admin/user/taxonomy_access',
          t('This action cannot be undone.'),
          t('Delete all'),
          t('Cancel'));
      }

      return;
    }
  }

  return drupal_not_found();
}

/**
 * Assembles a row of grant options for a term or default on the admin form.
 *
 * @param $grants
 *     An array of grants to use as form defaults, if any.
 */
function taxonomy_access_admin_build_row($grants = NULL) {
  $form['#title'] = '';
  $form['#tree'] = TRUE;
  foreach (array('view', 'update', 'delete') as $grant) {
    $form[$grant] = array(
      '#type' => 'radios',
      // 1: Allow, 0: Ignore, 2: Deny
      '#options' => array('1' => '', '0' => '', '2' => ''),
      '#default_value' => is_string($grants['grant_'. $grant]) ? $grants['grant_'. $grant] : '0',
      '#required' => TRUE,
    );
  }
  foreach (array('create', 'list') as $grant) {
    $form[$grant] = array(
      '#type' => 'checkbox',
      '#default_value' => is_string($grants['grant_'. $grant]) ? $grants['grant_'. $grant] : '0',
    );
  }
  return $form;
}

/**
 * Form for managing grants by role.
 *
 * @param $rid
 *     The role id.
 */
function taxonomy_access_admin_form($form_state, $rid = NULL) {
  // Fetch all default grants.
  $result = db_query('SELECT * FROM {term_access_defaults} WHERE rid = %d', $rid);
  while ($row = db_fetch_array($result)) {
    $default_grants[$row['vid']] = $row;
  }

  // If we are adding a role, no global default is set yet, so insert it now.
  if (empty($default_grants[0]) && isset($rid)) {
    // Assemble a $row object for Schema API.
    $row = new stdClass();
    $row->vid = 0;
    $row->rid = $rid;

    // Insert the row with defaults for all grants.
    drupal_write_record('term_access_defaults', $row);
  }

  // Fetch all grants.
  $result = db_query('SELECT * FROM {term_access} WHERE rid = %d', $rid);
  while ($row = db_fetch_array($result)) {
    $grants[$row['tid']] = $row;
  }

  $form['instructions'] = array(
    '#value' => _taxonomy_access_admin_instructions_html(),
    '#weight' => '20',
  );
  $form['rid'] = array('#type' => 'value', '#value' => $rid);
  $form['grants'] = $form['selected_terms'] = $form['selected_defaults'] = array('#tree' => TRUE);

  // Global default.
  $global_defaults = empty($default_grants[0]) ? NULL : $default_grants[0];
  $form['vocabs'][0]['#title'] = 'Global';
  $form['grants'][0][0] = taxonomy_access_admin_build_row($global_defaults);
  $form['selected_defaults'][0] = array(
     '#type' => 'checkbox',
     '#disabled' => TRUE,
     '#title' => t('<em>default</em>'),
     '#description' => t("can't be disabled without disabling TAC for this role"),
  );

  foreach (taxonomy_get_vocabularies() as $vid => $vocabulary) {
    $form['vocabs'][$vid]['#title'] = check_plain($vocabulary->name);
    if (isset($default_grants[$vid])) {
      $form['grants'][$vid][0] = taxonomy_access_admin_build_row($default_grants[$vid]);
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
          $form['grants'][$vid][$term->tid] = taxonomy_access_admin_build_row($grants[$term->tid]);
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
  // New grant row.
  if (isset($add_items)) {
    $form['new']['grants'] = taxonomy_access_admin_build_row();
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
 * Renders the permission matrix user form for a given user role.
 *
 * @return
 *     String containing rendered HTML form in table.
 */
function theme_taxonomy_access_admin_form($form) {
  $roles = _taxonomy_access_user_roles();
  $header = array(
    array( 'data' => t('Category'), 'colspan' => 3),
    array( 'data' => t('View'), 'colspan' => 4),
    array( 'data' => t('Update'), 'colspan' => 4),
    array( 'data' => t('Delete'), 'colspan' => 4),
    array( 'data' => t('Create')),
    array( 'data' => t('List')),
  );
  $sub_header = array(
    '&nbsp;<strong>'. t('<acronym title="Allow">A</acronym>') .'</strong>',
    '&nbsp;<strong>'. t('<acronym title="Ignore">I</acronym>') .'</strong>',
    '&nbsp;<strong>'. t('<acronym title="Deny">D</acronym>') .'</strong>',
    '&nbsp;',
  );
  $sub_header = array_merge(array('&nbsp;'), $sub_header, $sub_header, $sub_header);
  $sub_header = array_pad($sub_header, 15, '&nbsp;');
  $node_grant_types = array('view', 'update', 'delete');

  $radios = array('1' => t('Allow'), '0' => t('Ignore'), '2' => t('Deny'));

  drupal_set_title(t('Grants for %role', array('%role' => $roles[$form['rid']['#value']])));

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
          foreach (array('create', 'list') as $grant) {
            $row[] = array('data' => drupal_render($form['grants'][$vid][$tid][$grant]));
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
    foreach (array('create', 'list') as $grant) {
      $row[] = array('data' => drupal_render($form['new']['grants'][$grant]));
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
 * Submit handler for the administration form at admin/user/taxonomy_access.
 */
function taxonomy_access_admin_form_submit($form, &$form_state) {
  $values = $form_state['values'];

  switch ($values['op']) {

    case t('Delete selected'):
      if (is_array($values['selected_terms'])) {
        foreach ($values['selected_terms'] as $tid => $enabled) {
          if ($enabled) {
            $affected_nodes = _taxonomy_access_get_nodes_for_term($tid);
            db_query(
              'DELETE FROM {term_access} WHERE rid = %d AND tid = %d',
              $values['rid'], $tid
            );
            _taxonomy_access_cache_affected_nodes($affected_nodes);
          }
        }
      }
      if (is_array($values['selected_defaults'])) {
        foreach ($values['selected_defaults'] as $vid => $enabled) {
          if ($enabled) {
            $affected_nodes =
              _taxonomy_access_get_nodes_for_vocabulary($vid, $values['rid']);
            db_query(
              "DELETE FROM {term_access_defaults} WHERE rid = %d AND vid = %d",
              $values['rid'],
              $vid
            );
            _taxonomy_access_cache_affected_nodes($affected_nodes);
          }
        }
      }

      // Update any affected nodes.
      $affected_nodes = _taxonomy_access_cache_affected_nodes();
      if (!empty($affected_nodes)) {
        _taxonomy_access_node_access_update($affected_nodes);
      }

      break;

    case t('Add'):
      $new = $values['new'];
      list($type, $id) = explode(' ', $new['item']);
      if ($type == 'term') {
        if ($new['recursive'] == 1) {
          taxonomy_access_recursive_grant_update($id, $values['rid'], $new['grants']);
        }
        else {
          taxonomy_access_grant_update($id, $values['rid'], $new['grants']);
        }
      }
      elseif ($type == 'default') {
        taxonomy_access_defaults_update($id, $values['rid'], $new['grants']);
      }

      // Update any affected nodes.
      $affected_nodes = _taxonomy_access_cache_affected_nodes();
      if (!empty($affected_nodes)) {
        _taxonomy_access_node_access_update($affected_nodes);
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

            // If the grants for node access match the defaults, then we
            // can skip updating node access records for this row.
            $skip_nodes = TRUE;
            foreach (array('view', 'update', 'delete') as $op) {
              if ($defaults[$op] != $grants[$op]) {
                $skip_nodes = FALSE;
              }
            }
            if ($tid == 0) {
              taxonomy_access_defaults_update($vid, $values['rid'], $grants, $skip_nodes);
            }
            else {
              taxonomy_access_grant_update($tid, $values['rid'], $grants, $skip_nodes);
            }
          }
        }
      }

      // Update any affected nodes.
      $affected_nodes = _taxonomy_access_cache_affected_nodes();
      if (!empty($affected_nodes)) {
        _taxonomy_access_node_access_update($affected_nodes);
      }

      drupal_goto('admin/user/taxonomy_access');
      break;
  }
}

/**
 * Updates permissions for a role for a term.
 * Note: This function adds nodes to the affected nodes cache.
 * Callers should run  _taxonomy_access_node_access_update() on
 * _taxonomy_access_cache_affected_nodes() after all changes are processed.
 *
 * @param $tid
 *   The term to add the permission for.
 * @param $rid
 *   The role id to add the permission for.
 * @param $grants
 *   A hash of the grants in the form of $grants['perm'] = boolean
 *   A value of 1 will grant the permission for this user and term.
 * @param $skip_nodes
 *   A flag indicating whether to skip node updates when processing.
 */
function taxonomy_access_grant_update($tid, $rid = NULL, $grants = NULL, $skip_nodes = FALSE) {
  if (!isset($tid) OR !is_numeric($rid)) {
    return FALSE;
  }

  // Assemble a $row object for Schema API.
  $row = new stdClass();
  $row->tid = $tid;
  if (isset($rid)) {
    $row->rid = $rid;
  }
  if (isset($grants) && is_array($grants)) {
    foreach ($grants as $op => $value) {
      if (is_numeric($value)) {
        $grant_name = "grant_$op";
        $row->$grant_name = $value;
      }
    }
  }

  if (!$skip_nodes) {
    $affected_nodes = _taxonomy_access_get_nodes_for_term($tid);
  }

  // Delete old entries.
  db_query("DELETE FROM {term_access} WHERE tid=%d AND rid=%d", $tid, $rid);

  // Insert new entries.
  drupal_write_record('term_access', $row);

  // Add any affected nodes to the cache to be updated by the submit handler.
  if (!empty($affected_nodes)) {
    _taxonomy_access_cache_affected_nodes($affected_nodes);
  }
}

/**
 * Recursively updates permissions for a role for a term.
 *
 * @param $tid
 *   The term to add the permission for.
 * @param $rid
 *   The role id to add the permission for.
 * @param $grants
 *   A hash of the grants in the form of $grants['perm'] = boolean
 *   A value of 1 will grant the permission for this user and term.
 */
function taxonomy_access_recursive_grant_update($tid, $rid = NULL, $grants = NULL) {
  // First, process the original.
  taxonomy_access_grant_update($tid, $rid, $grants);

  // Process the children.
  $ran_tids = array(); // tids that have been processed.
  $run_tids = array($tid); // tids that are in the queue to be processed.
  while (count($run_tids) > 0) {
    foreach ($run_tids as $run_key => $run_tid) {
      // Some basic loop protection.
      if (!(array_search($run_tid, $ran_tids) === FALSE)) {
        drupal_set_message(
          t("Loop detected for tid %run_tid. Stopping.",
            array('%run_tid' => $run_tid)));
        $run_tids = array(); // stop the execution
      }
      else {
        $result = db_query('SELECT th.tid FROM {term_hierarchy} th WHERE th.parent = %d', $run_tid);
        // If this tid has children, update grants and queue the children
        while ($row = db_fetch_array($result)) {
          taxonomy_access_grant_update($row['tid'], $rid, $grants);
          $run_tids[] = $row['tid'];
        }

        // Remove this tid from the queue and mark as processed,
        unset($run_tids[$run_key]);
        $ran_tids[] = $run_tid;
      }
    }
  }
}

/**
 * Updates default permissions for a role for a vocabulary.
 * Note: This function adds nodes to the affected nodes cache.
 * Callers should run  _taxonomy_access_node_access_update() on
 * _taxonomy_access_cache_affected_nodes() after all changes are processed.
 *
 * @param $vid
 *   The vocab to add the permission for.
 * @param $rid
 *   The role id to add the permission to.
 * @param $grants
 *   A hash of the grants in the form of $grants['perm'] = boolean
 *   A value of 1 will grant the permission for this user and term.
 * @param $skip_nodes
 *   A flag indicating whether to skip node updates when processing.
 */
function taxonomy_access_defaults_update($vid, $rid = NULL, $grants = NULL, $skip_nodes = FALSE) {
  if (!is_numeric($vid) || (isset($rid) && !is_numeric($rid))) {
    return FALSE;
  }

  // Assemble $row object for Schema API.
  $row = new stdClass();
  $row->vid = $vid;
  $row->rid = $rid;
  if (isset($grants) && is_array($grants)) {
    foreach ($grants as $op => $value) {
      if (is_numeric($value)) {
        $grant_name = "grant_$op";
        $row->$grant_name = $value;
      }
    }
  }

  if (!$skip_nodes) {
    // If we are updating the global default, flag node access for rebuild.
    if ($vid === 0) {
      drupal_set_message(t("You have modified a global default, so you should see a message indicating that content access permissions need to be rebuilt.  However, you can wait to do this until you have finished any other changes you wish to make to the Taxonomy Access Permissions."), 'warning');
      node_access_needs_rebuild(TRUE);
    }
    // Otherwise, fetch a list of nodes tagged with the vocabulary.
    else {
      $affected_nodes = _taxonomy_access_get_nodes_for_vocabulary($vid, $rid);
    }
  }

  // Delete old entries.
  db_query("DELETE FROM {term_access_defaults} WHERE vid = %d AND rid = %d", $vid, $rid);

  // Insert new entries.
  drupal_write_record('term_access_defaults', $row);

  // Add any affected nodes to the cache to be updated by the submit handler.
  if (!empty($affected_nodes)) {
    _taxonomy_access_cache_affected_nodes($affected_nodes);
  }
}

/**
 * Generates HTML markup with form instructions for the admin form.
 *
 * @return
 *     Instructions HTML string.
 */
function _taxonomy_access_admin_instructions_html() {
  $instructions = '';
  $instructions .= ''
    . "<br /><br />"
    . "<div class=\"instructions\">"
    . "<h2>" . t("Explanation of fields") . "</h2>"
    . _taxonomy_access_grant_help_table()
    . "<p>"
    . t('Options for View, Update, and Delete are <em>Allow</em> (<acronym title="Allow">A</acronym>), <em>Ignore</em> (<acronym title="Ignore">I</acronym>), and <em>Deny</em> (<acronym title="Deny">D</acronym>).')
    . "</p>\n\n"
    . "<ul>\n"
    . "<li>"
    . t('<em>Deny</em> (<acronym title="Deny">D</acronym>) overrides <em>Allow</em> (<acronym title="Allow">A</acronym>) within this role.')
    . "</li>"
    . "<li>"
    . t('Both <em>Allow</em> (<acronym title="Allow">A</acronym>) and <em>Deny</em> (<acronym title="Deny">D</acronym>) override <em>Ignore</em> (<acronym title="Ignore">I</acronym>) within this role.')
    . "</li>"
    . "<li>"
    . t('If a user has <strong>multiple roles</strong>, an <em>Allow</em> (<acronym title="Allow">A</acronym>) from another role <strong>will</strong> override a <em>Deny</em> (<acronym title="Deny">D</acronym>) here.')
    . "</li>"
    . "</ul>\n\n"
    ;
  if (arg(4) > 2) { // Role other than Anonymous or Authenticated
    $instructions .= ''
      . "<p>"
      . t('<strong>Remember:</strong> This role <strong>will</strong> inherit permissions from the <em>authenticated user</em> role.  Be sure to <a href="@url">configure the authenticated user</a> properly.',
        array("@url" => url("admin/user/taxonomy_access/edit/2")))
      . "</p>\n\n"
      ;
  }
  $instructions .= ''
    . "<p>"
    . t('For more information and for troubleshooting guidelines, see the <a href="@help">help page</a> and the !readme.',
      array(
        '@help' => url('admin/help/taxonomy_access'),
        '!readme' => "<code>README.txt</code>"
      ))
    . "</p>\n\n"
    . "</div>\n\n"
    ;

  return $instructions;

}

/**
 * Assemble a table explaining each grant type for use in help documentation.
 *
 * @return
 *     Themed table.
 */
function _taxonomy_access_grant_help_table() {
  $header = array();

  $rows = array();
  $rows[] = array(
    array('header' => TRUE, 'data' => t("View")),
    "<p>"
    . t('Grants this role the ability to view nodes with the term.  (Users must also have this permission to see <em class="perm">nodes</em> with the term listed in Views.)')
    . "</p>"
    . "<p>"
    . t('The role must <strong>have</strong> <em class="perm">access content</em> permission on the <a href="@path">permissions administration form</a>.',
      array('@path' => url('admin/user/permissions', array('fragment' => 'module-node')))),
  );

  $rows[] = array(
    array('header' => TRUE, 'data' => t("Update") . ", " . t("Delete")),
    "<p>"
    . t("Grants this role the ability to edit or delete nodes with the term, respectively.")
    . "</p>"
    . "<p>"
    . t('The role must <strong>not</strong> have <em class="perm">edit any [type] content</em> or <em class="perm">delete any [type] content</em> permission on the <a href="@path">permissions administration form</a> if you wish to control them here.',
      array('@path' => url('admin/user/permissions', array('fragment' => 'module-node'))))
    ."</p>",
  );

  $rows[] = array(
    array('header' => TRUE, 'data' => t("Create")),
    "<p>"
    . t("Grants this role the ability to add the term to a node when creating or updating it.")
    . "</p>"
    . "<p>"
    . t('This does <strong>not</strong> give the role the ability to create nodes by itself; the role must <strong>have</strong> <em class="perm">create [type] content</em> permission on the <a href="@path">permissions administration form</a> in order to create new nodes.',
      array('@path' => url('admin/user/permissions', array('fragment' => 'module-node'))))
    ."</p>",
  );

  $rows[] = array(
    array('header' => TRUE, 'data' => t("List")),
    "<p>"
    . t("Whether this role can see the term listed on node pages and in lists, and whether the user can view the %taxonomy-term-page page for the term.",
      array(
        '%taxonomy-term-page' => "taxonomy/term/x"
      ))
    . "</p>"
    . "<p>" . t("This does <strong>not</strong> control whether the role can see the <em>nodes</em> listed in Views, only the <em>term</em>.") . "</p>",
  );

  return theme('table', $header, $rows, array('class' => 'grant_help'));
}