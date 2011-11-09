<?php
// $Id: node-og-group-post.tpl.php,v 1.3 2008/11/09 17:17:54 weitzman Exp $

/**
 * @file node-og-group-post.tpl.php
 * 
 * Og has added a brief section at bottom for printing links to affiliated groups.
 * This template is used by default for non group nodes.
 *
 * Theme implementation to display a node.
 *
 * Available variables:
 * - $title: the (sanitized) title of the node.
 * - $content: Node body or teaser depending on $teaser flag.
 * - $picture: The authors picture of the node output from
 *   theme_user_picture().
 * - $date: Formatted creation date (use $created to reformat with
 *   format_date()).
 * - $links: Themed links like "Read more", "Add new comment", etc. output
 *   from theme_links().
 * - $name: Themed username of node author output from theme_user().
 * - $node_url: Direct url of the current node.
 * - $terms: the themed list of taxonomy term links output from theme_links().
 * - $submitted: themed submission information output from
 *   theme_node_submitted().
 *
 * Other variables:
 * - $node: Full node object. Contains data that may not be safe.
 * - $type: Node type, i.e. story, page, blog, etc.
 * - $comment_count: Number of comments attached to the node.
 * - $uid: User ID of the node author.
 * - $created: Time the node was published formatted in Unix timestamp.
 * - $zebra: Outputs either "even" or "odd". Useful for zebra striping in
 *   teaser listings.
 * - $id: Position of the node. Increments each time it's output.
 *
 * Node status variables:
 * - $teaser: Flag for the teaser state.
 * - $page: Flag for the full page state.
 * - $promote: Flag for front page promotion state.
 * - $sticky: Flags for sticky post setting.
 * - $status: Flag for published status.
 * - $comment: State of comment settings for the node.
 * - $readmore: Flags true if the teaser content of the node cannot hold the
 *   main body content.
 * - $is_front: Flags true when presented in the front page.
 * - $logged_in: Flags true when the current user is a logged-in member.
 * - $is_admin: Flags true when the current user is an administrator.
 *
 * @see template_preprocess()
 * @see template_preprocess_node()
 */
 //kpr($variables);
?>
<div class="row">
	<div class="topRow">
		<?php //kpr($node); ?>
		<a  href="<?php print $node_url; ?>"><?php print $title; ?></a>
		<br/><?php print  $content; ?>
		</div>
	<div class="baseRow baseline">
		<?php 
			echo t('By').$name;			
			echo t('at').' '.date('j/m/Y H:i', $created);
			echo ' | ';  
			
			if($comment_count == 0){
				echo t('No posts').'.';
			}else{
				
				echo $comment_count.' '.t('posts').' | ';
				$number = 5;
				$result = db_query_range(db_rewrite_sql("SELECT nc.last_comment_uid FROM {node_comment_statistics} nc WHERE nc.comment_count > 0 AND nc.nid ="
				.$nid." ORDER BY nc.last_comment_timestamp DESC", 'nc'), 0, $number);
			
				while ($row = db_fetch_object($result)) {
					$last_comment_uid = $row->last_comment_uid;
					$last_comment_user = user_load($last_comment_uid);					
				}
				echo t('Last post by').' ';
			
			
		?>
		<a href="<?php print url("user/".($last_comment_uid)); ?>"><?php echo $last_comment_user->name.' '; ?></a>
		<?php echo t('at').' '.date('j/m/Y H:i', $last_comment_timestamp); ?>
		<?php } ?>		
	</div>	
</div>
<?php if ($links): ?> 
	<div class="links baseline"><?php print $links; ?></div>
<?php endif; ?>

