<?php
// $Id: views_handler_field_is_online.inc,v 1.1.2.3 2009/02/13 03:18:36 liammcdermott Exp $

/**
 * @file
 * User Stats is user online handler.
 */

/**
 * Is user online handler. Useful for newbies who don't know how to do this
 * using filters.
 */
class views_handler_field_is_online extends views_handler_field_boolean {
  function query() {
    $sql = "(". time() ." - users.access) < 900";
    // We liberally steal from views_handler_sort_formula here.
    $this->ensure_my_table();
    $this->field_alias = $this->query->add_field(NULL, $sql, $this->table_alias .'_'. $this->field);
  }

  function option_definition() {
    $options = parent::option_definition();

    $options['type'] = array('default' => 'online-offline');

    return $options;
  }

  /**
   * Add the online-offline type to options form.
   */
  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    $form['type']['#options']['online-offline'] = t('Online/Offline');
  }

  function render($values) {
    $value = $values->{$this->field_alias};
    if (!empty($this->options['not'])) {
      $value = !$value;
    }
    if ($this->options['type'] == 'online-offline') {
      return $value ? t('Online') : t('Offline');
    }
    else {
      return parent::render($values);
    }
  }
}
