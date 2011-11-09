<?php if(!$is_contact) : ?>
<?php if($node && $node->type == 'book'): ?>
<div class="espace">
<div class="plan_wiki">
<?php 
	function draw_plan_book($tree){
		$output = '';
		$items = array();
		foreach ($tree as $data) {
			if (!$data['link']['hidden']) {
				$items[] = $data;
			}
		}
		$num_items = count($items);
		foreach ($items as $i => $data) {
			if ($i == 0) {
				$output .= '<ol>';
			}			
			if ($data['below']) {
				$output .= '<li>'.theme('menu_item_link', $data['link']).draw_plan_book($data['below']).'</li>';
			}
			else {
				$output .= '<li>'.theme('menu_item_link', $data['link']).'</li>';
			}
			if ($i == $num_items - 1) {
				$output .= '</ol>';
			}
		}
		return $output;
	}

	$current_bid = 0;
	$current_bid = empty($node->book['bid']) ? 0 : $node->book['bid'];
	if ($current_bid) {
        // Only display this block when the user is browsing a book.
        $title = db_result(db_query(db_rewrite_sql('SELECT n.title FROM {node} n WHERE n.nid = %d'), $node->book['bid']));
        // Only show the block if the user has view access for the top-level node.
        if ($title) {
			$tree = menu_tree_all_data($node->book['menu_name'], $node->book);
			// There should only be one element at the top level.
			$data = array_shift($tree);
			echo '<h5>'.t('Contents of').' '.$data['link']['title'].'</h5>';
			//kpr( $data);
			$book_tree = ($data['below']) ? draw_plan_book($data['below']) : '';			
        }
    }
      
?>
		<div class="contentWikiPlan">
			<?php echo $book_tree; ?>
		</div>  
		<div class="bottomWikiPlan"></div> 
	</div>     
</div> 	
<?php else : ?>   
	<div class="espace">
		<?php if (!empty($block->subject)): ?>
		  <h5 class="title block-title"><?php print $block->subject; ?></h5>
		<?php endif; ?>

		<div class="espaceMidle">
		  <?php print $block->content; ?>
		</div>
		<?php print $edit_links; ?>		
	</div>
<?php endif; ?>
<?php endif; ?>
	
	
