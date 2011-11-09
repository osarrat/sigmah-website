<?php
// $Id: views_handler_sort_is_online.inc,v 1.1.2.3 2009/02/13 03:18:36 liammcdermott Exp $

/**
 * @file
 * User Stats is user online sort handler.
 */

/**
 * Is user online sort handler. Useful for newbies who don't know how to do this
 * using filters.
 */
class views_handler_sort_is_online extends views_handler_sort {
  function query() {
    $sql = "(". time() ." - users.access) < 900";
    $this->ensure_my_table();
    $this->query->add_orderby(NULL, $sql, $this->options['order'], $this->table_alias .'_'. $this->field);
  }
}
