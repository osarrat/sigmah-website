<?php
// $Id: node-og-group.tpl.php,v 1.3 2008/10/29 20:04:41 dww Exp $

/**
 * @file node-og-grouo.tpl.php
 * 
 * Og has not modified this at all. It is same as node.tpl.php.
 * This template is used by default for group nodes.
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
?>
<div id="node-<?php print $node->nid; ?>" class="node<?php if ($sticky) { print ' sticky'; } ?><?php if (!$status) { print ' node-unpublished'; } ?> clear-block">

<?php print $picture ?>

<?php if (!$page): ?>
  <h2><a href="<?php print $node_url ?>" title="<?php print $title ?>"><?php print $title ?></a></h2>
<?php endif; ?>

  <div class="meta">
  <?php if (isset($submitted)): ?>
    <span class="submitted"><?php print $submitted ?>	
	</span>
  <?php endif; ?> 
	<?php 
	$nb_posts = user_stats_get_stats('post_count', $node->uid);
	if($nb_posts < 1):?>
	<div class="forum-post-10">Post count: 
	<?php echo $nb_posts; ?>
	</div>
	<?php elseif($nb_posts < 2):?>
	<div class="forum-post-50">Post count: 
	<?php echo $nb_posts;?> 
	</div>
	<?php else: ?>
	<div class="forum-post-100">Post count: 
	<?php echo $nb_posts;?>
	</div>
	<?php	//echo '<img src="/imgs/Etoile-orange.png" alt="plus de 50 posts" />';
		//echo $image = theme('image', '/imgs/Etoile-orange.png');
	endif;
	?>, 
	Last log since : 
	<?php print user_stats_get_stats('login_days', $node->uid); ?> 
	days,
	<?php $online = user_stats_get_stats('online', $node->uid); if($online){ print $online;} else{ print 'offline';} ?>
  <?php if ($terms): ?>
    <div class="terms terms-inline"><?php print $terms ?></div>
  <?php endif;?>
  </div>

  <div class="content">
    <?php print $content ?>
  </div>

  <?php print $links; ?>
</div> 