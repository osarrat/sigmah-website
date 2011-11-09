<a href="sigmahDev/node/12" rel="lightframe">View</a>

<!--a href="/sigmahDev/node/12" class="extrigger">View</a>
<!--div class="jqmWindow" id="ex2">
<a href="#" class="jqmClose">Close</a>
Please wait... <img src="inc/busy.gif" alt="loading" />
</div-->

<div class="jqmAlert" id="ex3b">
  <div class="jqmAlertWindow" id="ex3b" >
    <div class="jqmAlertTitle clearfix">
      <h1>Did you know?</h1>
	  <a href="#" class="jqmClose"><em>Close</em></a>
    </div> 
    <div class="jqmAlertContent">
      <p>Please wait... <img src="inc/busy.gif" alt="loading" /></p>
    </div>
  </div>
</div>

<!--a href="#dialog" class="jqModal">View...</a-->
<!--div class="jqmWindow" id="dialog">

<a href="#" class="jqmClose">Close</a>
<hr>
<em>READ ME</em> 
This is a "vanilla plain" jqModal window. Behavior and appeareance extend far beyond this.
The demonstrations on this page will show off a few possibilites. I recommend walking
through each one to get an understanding of jqModal <em>before</em> using it.

<br /><br />
You can view the sourcecode of examples by clicking the Javascript, CSS, and HTML tabs.
Be sure to checkout the <a href="README">documentation</a> too!

<br /><br />
<em>NOTE</em>; You can close windows by clicking the tinted background known as the "overlay".
Clicking the overlay will have no effect if the "modal" parameter is passed, or if the
overlay is disabled.
</div-->
<?php 
    /*kpr($mission);
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
	<?php $online = user_stats_get_stats('online', $node->uid); if($online){ print $online;} else{ print 'offline';} */?>
<?php

if (!empty($mission)) {
?>
  <div id="mission" class="og-mission"><?php print $mission; ?></div>
<?php } ?> 
