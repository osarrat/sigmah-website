<div class="<?php print $classes .' '. $zebra; ?> clearfix">
  <div class="comment-inner commentaire">	
    <?php if ($new) : ?>
      <!--span class="new"><?php print drupal_ucfirst($new); ?></span-->
    <?php endif; ?>

    <?php print $picture; ?>
	<div class="main_comment">
		<?php if($submitted): ?>
		<div class="top_comment baseline jaune">
			<?php print $submitted; ?>
		</div>
		<?php endif; ?>
		<div class="content_comment">
		
			<?php if ($title): ?>
				<div class="top_content_comment"><?php print $title ?></div>
			<?php endif; ?>		
			<?php print $content ?>
			<?php if ($signature): ?>
			<div class="user-signature clearfix">
			  <?php print $signature; ?>
			</div>
		  <?php endif; ?>
		</div>
		<div class="addComment baseline"><?php if ($links): ?>	
		<?php print $links; ?>	 
	<?php endif; ?></div>
	</div>
		
	<div class="right_content">
		<a class="baseline pseudo_comment 
		<?php 
		if(user_stats_get_stats('post_count', $comment->uid) < 10): 
			echo 'forum-post-10';
		elseif(user_stats_get_stats('post_count', $comment->uid) < 50): 
			echo 'forum-post-50';
		elseif(user_stats_get_stats('post_count', $comment->uid) < 100): 
			echo 'forum-post-100';
		endif; ?>" href="<?php print url("user/".($comment->uid)); ?>">
		<?php echo $comment->registered_name; ?>
		</a>
		<ul>
			<li class="baseline first-child">			
			<?php $online = user_stats_get_stats('online', $comment->uid); 
			if($online): ?>
				<span class="jaune">
				<?php print t('online'); ?>
				</span>
			<?php else: ?>
				<?php echo t('Last log since').' : '; ?><span class="jaune">
				<?php print user_stats_get_stats('login_days', $comment->uid).' '.t('days'); ?>
				</span>
			<?php endif;?>
			</li>
			<li class="baseline"><?php echo t('Post').' : '; ?>
			<span class="jaune"><?php print user_stats_get_stats('post_count', $comment->uid); ?></span></li>
			<li class="baseline"><?php echo t('Joined').' : '; ?><span class="jaune">
			<?php 
			$comment_user = user_load($comment->uid);
			echo t('at').' '.date('j/m/Y H:i', $comment_user->created); ?></span>
			</li>		
			
			<li class="baseline">
				<?php echo t('Location').' : ';	?>
				<span class="jaune">
					<?php
						$profile = profile_load_profile($comment_user);
						echo $comment_user->profile_country;
					?>
				</span>
			</li>

		</ul>
	</div>
  </div> <!-- /comment-inner -->  
  
</div> <!-- /comment -->
