<?php
// $Id: skype_status_handler_field_user.inc,v 1.3 2010/04/06 14:57:22 nicholasalipaz Exp $

class skype_status_handler_field_user extends views_handler_field_user {
  function option_definition() {
    $options = parent::option_definition();
    $options['skype_status_format'] = array('default' => 'skype_status_button');
    return $options;
  }

  /**
   * Provide link to node option
   */
  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    $opts = array(
      'skype_status_button' => 'Skype Button',
      'skype_status_id' => 'Skype ID (plain text)',
    );

    $form['skype_status_format'] = array(
      '#title' => t('Format'),
      '#description' => t('This will override any other link you have set.'),
      '#type' => 'select',
      '#options' => $opts,
      '#default_value' => $this->options['skype_status_format'],
    );
    unset($form['link_to_user']);
  }

  function render($values) {
    if (empty($values->{$this->field_alias})) return t('Skype ID not set for');
    switch ($this->options['skype_status_format']):
      case 'skype_status_button':
        $buttonstyle = variable_get('skype_status_buttonstyle', 'bigclassic');
        $status = skype_status_getstatus(rawurlencode(check_plain($values->{$this->field_alias})));
        $output = theme('skype_status', check_plain($values->{$this->field_alias}), $buttonstyle, $status);
        return $output;
      case 'skype_status_id':
        return check_plain($values->{$this->field_alias});
    endswitch;
  }
}
