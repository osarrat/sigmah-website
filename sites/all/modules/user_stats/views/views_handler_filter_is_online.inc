<?php
// $Id: views_handler_filter_is_online.inc,v 1.1.2.3 2009/02/13 03:18:36 liammcdermott Exp $

/**
 * @file
 * User Stats is user online sort handler.
 */

/**
 * Is user online sort handler. Useful for newbies who don't know how to do this
 * using filters.
 */
class views_handler_filter_is_online extends views_handler_filter_boolean_operator {
  function query() {
    $sql = "(". time() ." - users.access) < 900";
    $this->ensure_my_table();
    $this->query->add_where($this->options['group'], $sql);
    $this->field_alias = $this->table_alias .'_'. $this->field;
  }

  /**
   * Override default True/False options.
   */
  function get_value_options() {
      $this->value_options = array(1 => t('Online'), 0 => t('Offline'));
  }
}
