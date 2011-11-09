<?php  if(!$is_contact) : //blocks must not appear on contact page?>
<?php if (!empty($block->subject) && $block->subject == 'Who\'s online'){ 

	// Count users active within the defined period.
          $interval = time() - variable_get('user_block_seconds_online', 900);

          // Perform database queries to gather online user lists.  We use s.timestamp
          // rather than u.access because it is much faster.
          $anonymous_count = sess_count($interval);
          $authenticated_users = db_query('SELECT DISTINCT u.uid, u.name, s.timestamp FROM {users} u INNER JOIN {sessions} s ON u.uid = s.uid WHERE s.timestamp >= %d AND s.uid > 0 ORDER BY s.timestamp DESC', $interval);
          $authenticated_count = 0;
          $max_users = variable_get('user_block_max_list_count', 10);
          $items = array();
          while ($account = db_fetch_object($authenticated_users)) {
            if ($max_users > 0) {
              $items[] = $account;
              $max_users--;
            }
            $authenticated_count++;
          }
			
          // Display a list of currently online users.
          $max_users = variable_get('user_block_max_list_count', 10);
          if ($authenticated_count && $max_users) {
			$user_names = Array(); 
		    foreach($items as $item){
				$auth_user = user_load($item->uid);
				if(!in_array($item->name, $user_names)){
					$user_names[] = $item->name;
					if(!empty($auth_user->profile_skype)){
						$buttonstyle = variable_get('skype_status_buttonstyle', 'bigclassic');
						$status = skype_status_getstatus(rawurlencode($auth_user->profile_skype));
						$skype_output = theme('skype_status', $auth_user->profile_skype, $buttonstyle, $status);
						$output .= '<tr><td class="nom"><a href="'.url("user/".($item->uid)).'">'.$item->name.'</a></td><td class="skype">
						'.$skype_output.'<a href="skype:'.$auth_user->profile_skype. '?call">'.t('Skype me!').'</a></td></tr>';
					}else{
						$output .= '<tr><td class="nom"><a href="'.url("user/".($item->uid)).'">'.$item->name.'</a></td><td class="skype"></td></tr>';
					}					
				}										
			}
          }
	?>
	<div class="espace">
        	<H5 class="account"><?php echo $user->name; ?></h5>
            <div class="espaceMidle">
            <?php echo t('Connected members');?>
            <table>
            <?php echo $output; ?>           
            </table>
            <hr />
            <div class="right baseline"><a href="<?php print url("user/".($user->uid)); ?>"><?php echo t('My account');?></a>  
            		
                    <span class="deco">
                       <a href="<?php echo url('logout'); ?>"><?php echo t('Log out'); ?></a>
                    </span>
                  </div>
            </div>
    </div>
<?php }elseif($block->delta === '0') {?>
	<span id="ident">
	<?php if (!empty($block->subject)): ?>
		<?php if(isset($logged_in) && $logged_in): ?>
			<a class="log" href="<?php print url("user/".($user->uid)); ?>"><?php print $block->subject; ?></a>
		<?php endif; ?>
		<?php if(!$logged_in): ?>
			<a class="log" href="<?php print url("user/login"); ?>"><?php print $block->subject; ?></a>
		<?php endif; ?>
	<?php endif; ?>
    </span>
	<?php if($logged_in): ?>
		<span id="facebook" class="deco">		
				<a href="<?php echo url('logout'); ?>"><?php echo t('Log out'); ?></a>		
		</span>
	<?php endif; ?>
    <?php //print $edit_links; ?>
<?php }elseif($block->delta === '1' && $logged_in) { ?>
	<span id="ident">
	<?php if (!empty($block->subject)): ?>
        <a class="log" href="<?php print url("user/".($user->uid)); ?>"><?php print $block->subject; ?></a>
	<?php endif; ?>
    </span>
    <span id="facebook" class="deco">
        <a href="<?php echo url('logout'); ?>"><?php echo t('Log out'); ?></a>
    </span>
    <?php //print $edit_links; ?>
<?php }?>
<?php endif; ?>
	