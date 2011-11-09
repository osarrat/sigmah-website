<?php
// $Id: taxonomy_access_handler_filter_delete.inc,v 1.1.2.1 2010/09/03 21:25:55 xjm Exp $

/**
 * @file
 * Provides the Views filter handler for TAC's Delete grant.
 */

/**
 * Handler to filter the view to only nodes with a grant_delete from TAC.
 */
class taxonomy_access_handler_filter_delete extends views_handler_filter {

  function admin_summary() { }
  function operator_form() { }

  // Custom implementation of query() to add the proper joins and role subquery.
  function query() {
    $table = $this->ensure_my_table();

    // Join {node} to {term_node}.
    $join = new views_join();
    $join->construct('term_node', $this->table_alias, 'nid', 'nid');
    $this->query->ensure_table('term_node', $this->relationship, $join);

    // Join {term_node} to {term_access} for grant data.
    $join = new views_join();
    $join->construct('term_access', 'term_node', 'tid', 'tid');
    $this->query->ensure_table('term_access', $this->relationship, $join);

    // Where one of the user's roles has grant_delete for this term record.
    $this->query->add_where($this->options['group'], "term_access.grant_delete = 1 AND term_access.rid IN (SELECT rid FROM users_roles WHERE uid = ***CURRENT_USER***)");
  }
}
