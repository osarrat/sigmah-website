<?php
class webfm_views_handler_file_name extends views_handler_field {
	
	function query() {
		$this->ensure_my_table ();
		// Add the field.
		$this->field_alias = $this->query->add_field ( $this->table_alias, 'fpath' );
		
		$this->add_additional_fields ();
	}
	
	function render($values) {
		
		//$firephp = FirePHP::getInstance(true);
  	//$firephp->log($values);
   
    if(variable_get('webfm_display_title', '') && $values->webfm_file_ftitle) {
      $fname = urldecode($values->webfm_file_ftitle);
    }
    else {
      $fname = strrev(substr(strrev($values->webfm_file_fpath), 0, strpos(strrev($values->webfm_file_fpath), '/')));
    }
    
    if ($this->options['mode'] == 'link') {
    	return l(check_plain($fname), 'webfm_send/'.$values->fid);
    } 
    
    return check_plain($fname);
	}
	
	function options_definitions() {
	   //$options = parent::option_definition();

		$options['label'] = array('default' => $this->definition['title'], 'translatable' => TRUE);
		$options['mode'] = array('default' => 'link');
    
		return $options;
  }
	
	function options_form(&$form, &$form_state) {
     
		//parent::options_form($form, $form_state);
		
		$form['label'] = array(
       '#type' => 'textfield',
       '#title' => t('Label'),
       '#default_value' => isset($this->options['label']) ? $this->options['label'] : '',
       '#description' => t('The label for this field that will be displayed to end users if the style requires it.'),
    );
				
		$form['mode'] = array(
	       '#type' => 'select',
 	       '#title' => t('Output Mode'),
			   '#options' => array(
			      'link' => t('With links'),
          	'nolink' => t('Without links')
				)   
     ); 
	}
}
?>