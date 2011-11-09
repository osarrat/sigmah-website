<div style="font-family: Helvetica, sans-serif;">
  <?php
  $spaces = '';
  for ($i = 1; $i < intval($data['depth']); $i++) {
    $spaces .= '&nbsp;&nbsp;&nbsp;&nbsp;';
  }
  ?>

  <?php
  $style = '';
  
  if ($data['depth'] == 1) {
    $style = 'color: #40271A; font-size: 90px; text-align: center; font-weight: bold;';
  }

  if ($data['depth'] == 2) {
    $style = 'color: #40271A; font-size: 75px; font-weight: bold;';
  }

  if ($data['depth'] == 3) {
    $style = 'color: #40271A; font-size: 60px; font-weight: bold;';
  }

  if ($data['depth'] == 4) {
    $style = 'color: #40271A; font-size: 45px; font-weight: bold;';
  }

  if ($data['depth'] == 5) {
    $style = 'color: #40271A; font-size: 30px; font-weight: bold;';
  }

  if ($data['depth'] == 6) {
    $style = 'color: #40271A; font-size: 15px; font-weight: normal; font-style: italic;';
  }
  ?>

  <h1 <?php if ($style != ''): ?>style="<?php print $style; ?>" <?php endif; ?>><?php print $spaces . $data['title']; ?></h1>
  <?php 
  if(!(strripos($data['title'], 'Page de livre 1') === false)){$data['body'] = '<div id="topPersonal">Top</div>'.$data['body'];}
  if(!(strripos($data['title'], 'fhdfghdfg') === false)){$data['body'] = $data['body'].'naaaaaaaa';}
  print $data['body']; ?>
</div>