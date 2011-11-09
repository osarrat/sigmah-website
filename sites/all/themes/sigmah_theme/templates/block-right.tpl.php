<?php if(!$is_contact) : ?>
<?php if (!empty($block->subject) && ($block->subject == 'My groups' || $block->subject == 'Forums')){ ?>
	<div class="forum_list">
		<h5><a href="og"><?php echo strip_tags($block->subject); ?></a></h5>
		<div class="content">
			<?php print $block->content; ?>
		</div>
		
		<form action="forum.php" method="get">
			<input type="recherche" type="text" onfocus="if(this.value=='Rechercher')this.value='';" onblur="if(this.value=='')this.value='Rechercher';" id="forumRec" value="Rechercher" />
			<input type="submit"  id="forumButton" value="OK" name="recherche" />
		</form>
		<?php print $edit_links; ?>
	</div> <!-- /block -->
<?php }else{ ?>
	<div class="espace">
		<?php if (!empty($block->subject)): ?>
		  <h5 class="title block-title"><?php print strip_tags($block->subject); ?></h5>
		<?php endif; ?>

		<div class="espaceMidle">
		  <?php print $block->content; ?>
		</div>

		<?php print $edit_links; ?>
	</div>
<?php } ?>
<?php endif; ?>