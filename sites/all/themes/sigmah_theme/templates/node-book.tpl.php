<?php //kpr($variables); ?>
<div class="node <?php print $classes; ?>" id="node-<?php print $node->nid; ?>">
  <div class="node-inner">

    <?php if (!$page): ?>
      <h2 class="title"><a href="<?php print $node_url; ?>"><?php print $title; ?></a></h2>
    <?php endif; ?>

    <?php print $picture; ?>

    <?php if($submitted): ?>
    <div class="top_comment baseline jaune">
    <?php //print $submitted; ?>
    <?php
    //Display a more Wiki like information blurb
    //about who last edited and when for this node.
    $nodeid = $node->nid;
    if (isset($nodeid)) {
      $result = db_query("SELECT u.name AS last_editor
          FROM {node_revisions} nr, {users} u
          WHERE nr.uid = u.uid
          AND nr.nid = " .$nodeid. "
          ORDER BY timestamp DESC
          LIMIT 1");
      $resultset = db_fetch_object($result);
      // Checking if the user has required permissions to see the profile of the user
      $userLink = $resultset->last_editor ;
      if(user_access("access user profiles")){
        // if the current viewer has permissions to access the profile of the user who edited the last profile, then linking the last editor name to the profile
        $resultset->last_editor = l($resultset->last_editor, 'users/' . $resultset->last_editor);
      } 
      // access check complete
      echo t("Last modified by ").$resultset->last_editor;
      echo t(" on ").format_date($changed);
    } 
?>
		</div>
	<?php endif; ?>
	
    <div class="content">
      <?php print $content; ?>
    </div>

    <?php if ($terms): ?>
      <div class="taxonomy"><?php print $terms; ?></div>
    <?php endif;?>

    <?php if ($links): ?> 
      <div class="links baseline"> <?php print $links; ?></div>
    <?php endif; ?>

  </div> <!-- /node-inner -->
</div> <!-- /node-->
