<?php
// $Id: views-view.tpl.php,v 1.13.2.2 2010/03/25 20:25:28 merlinofchaos Exp $
/**
 * @file views-view.tpl.php
 * Main view template
 *
 * Variables available:
 * - $classes_array: An array of classes determined in
 *   template_preprocess_views_view(). Default classes are:
 *     .view
 *     .view-[css_name]
 *     .view-id-[view_name]
 *     .view-display-id-[display_name]
 *     .view-dom-id-[dom_id]
 * - $classes: A string version of $classes_array for use in the class attribute
 * - $css_name: A css-safe version of the view name.
 * - $css_class: The user-specified classes names, if any
 * - $header: The view header
 * - $footer: The view footer
 * - $rows: The results of the view query, if any
 * - $empty: The empty text to display if the view is empty
 * - $pager: The pager next/prev links to display, if any
 * - $exposed: Exposed widget form/info to display
 * - $feed_icon: Feed icon to display, if any
 * - $more: A link to view more, if any
 * - $admin_links: A rendered list of administrative links
 * - $admin_links_raw: A list of administrative links suitable for theme('links')
 *
 * @ingroup views_templates
 */
 //kpr($view->result);
?>
<?php 
	$number = 3;
	function showRowBottom($node){
		echo t('By').' '.$node->name.' ';			
		echo t('at').' '.date('j/m/Y H:i', $node->created);
		echo ' | ';  
		
		if($node->comment_count == 0){
			echo t('No posts').'.';
		}else{
			
			echo $node->comment_count.' '.t('posts').' | ';
			$comment_result = db_query_range(db_rewrite_sql("SELECT nc.last_comment_uid FROM {node_comment_statistics} nc WHERE nc.comment_count > 0 AND nc.nid ="
			.$node->nid." ORDER BY nc.last_comment_timestamp DESC", 'nc'), 0, 1);				
			while ($comment_row = db_fetch_object($comment_result)) {				
				$last_comment_uid = $comment_row->last_comment_uid;				
				$last_comment_user = user_load($last_comment_uid);		
			}
			echo t('Last post by').' ';
			echo '<a href="'.url("user/".($last_comment_uid)).'">'.$last_comment_user->name.' '.'</a>'.
			t('at').' '.date('j/m/Y H:i', $node->last_comment_timestamp);
		}
	}
?>
<div class="<?php print $classes; ?>"> 
	<?php $forum_id =5; $forum = taxonomy_get_term($forum_id);?>
	<h4><?php echo $forum->name; ?></h4>
	<?php if ($admin_links): ?>
		<div class="views-admin-links views-hide">
		  <?php print $admin_links; ?>
		</div>
	<?php endif; ?>
	<p class="jaune">
		<?php 
		//2010-11-02: add forum description printing 
		print $forum->description; ?>
	</p>
	<div id="forum">
		<?php if($view->result): ?>	
		<?php foreach ($view->result as $selectedOG): ?>
			<h5><?php echo $selectedOG->node_title ; ?></h5>
			<div class="forum">
			<?php 
			
			$result = db_query_range(db_rewrite_sql("SELECT oga.nid FROM {og_ancestry} oga 
			WHERE oga.group_nid ="
				.$selectedOG->nid, 'oga'), 0, $number);
			$row = db_fetch_object($result); 
			$og_discussion = node_load($row->nid); 
			if($og_discussion->status == 1):
			?>
				<div class="row first">
					<div class="topRow">
						<a  href="<?php echo url('node/'.$og_discussion->nid); ?>"><?php print $og_discussion->title; ?>
						<br/><?php echo $og_discussion->teaser; ?></a>
					</div>
					<div class="baseRow baseline baseline"><?php showRowBottom($og_discussion); ?></div>
				</div>			
			<?php 
			endif;
			while ($row = db_fetch_object($result)) {
				//kpr(node_load($row->nid));	
				$og_discussion = node_load($row->nid); 
				if($og_discussion->status == 1): ?>		
					<div class="row">
						<div class="topRow">
							<a  href="<?php echo url('node/'.$og_discussion->nid); ?>"><?php print $og_discussion->title; ?></a>
							<br/><?php echo $og_discussion->teaser; ?>
						</div>
						<div class="baseRow baseline"><?php showRowBottom($og_discussion); ?></div>
					</div>
			<?php 
				endif;
			}			
			?>
			</div>
            <p class="baseline right"><a href="<?php echo url('node/'.$selectedOG->nid); ?>"><?php echo t('See all'); ?></a></p>            
		<?php endforeach; ?>
		<?php endif; ?>
	</div>
	
	<?php if ($pager): ?>
		<?php print $pager; ?>
	<?php endif; ?>

	<?php if ($attachment_after): ?>
		<div class="attachment attachment-after">
		  <?php print $attachment_after; ?>
		</div>
	<?php endif; ?>

	<?php if ($more): ?>
		<?php print $more; ?>
	<?php endif; ?>

	<?php if ($footer): ?>
		<div class="view-footer">
		  <?php print $footer; ?>
		</div>
	<?php endif; ?>

	<?php if ($feed_icon): ?>
		<div class="feed-icon">
		  <?php print $feed_icon; ?>
		</div>
	<?php endif; ?>

</div> <?php /* class view */ ?>