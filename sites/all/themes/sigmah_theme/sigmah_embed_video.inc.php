<?php
/*
     Template to display an embedded video
     
     
     Parameters with an array which should be declared before including this file :
       $sigmahEmbedVideoParamsArray = array(
            'language' => 'en',
            'dailymotion' => 'xkve9n',         
            'dailymotion-group' => array ('name'=>'Sigmah Launch Conference 2011', 
                                          'code'=>'sigmah-launch-conference-2011'),
            'ogv' => '01-background.ogv',
            'ogv-group' => 'launch-conference-2011',
            'extra-link' => array ('label'=>'French version',
                                  'url'=>'http://www.dailymotion.com/video/xkuvnw')
        );
     
     
*/
?>
    <table style="border-spacing:0;"> 
      <tbody>    
        <tr>      
          <td style="width:160px">&nbsp;</td>      
          <td style="width:320px;background-color:#494748;text-align:center;color:#AF9972;">
            <?php
              if(array_key_exists('dailymotion-group', $sigmahEmbedVideoParamsArray) && is_array($sigmahEmbedVideoParamsArray['dailymotion-group']) ):
            ?>
            <?php if ($sigmahEmbedVideoParamsArray['language']=='en') echo "Video part of"; else echo "Vid&eacute;o de"; ?>
            <a href="http://www.dailymotion.com/group/<?php echo $sigmahEmbedVideoParamsArray['dailymotion-group']['code']; ?>" target="_blank" style="color:#AF9972;">
            <?php echo $sigmahEmbedVideoParamsArray['dailymotion-group']['name']; ?></a> <br />
            <?php
              endif;
            ?>
            <iframe frameborder="0" width="320" height="240" src="http://www.dailymotion.com/embed/video/<?php echo $sigmahEmbedVideoParamsArray['dailymotion']; ?>?foreground=%23FFFFFF&highlight=%23F5B235&background=%23494748">
            </iframe>
            <?php
              if(array_key_exists('ogv', $sigmahEmbedVideoParamsArray)):
            ?>
            <br />
            <a href="http://www.sigmah.org/videos/<?php if(array_key_exists('ogv-group', $sigmahEmbedVideoParamsArray)) echo $sigmahEmbedVideoParamsArray['ogv-group']."/"; ?><?php echo $sigmahEmbedVideoParamsArray['ogv']; ?>" style="color:#AF9972;font-style: italic;">
            <?php if ($sigmahEmbedVideoParamsArray['language']=='en') echo "Download this video in free format (.ogv)"; else echo "T&eacute;l&eacute;charger cette vid&eacute;o en format libre (.ogv)"; ?>
            </a>
            <?php
              endif;
              if(array_key_exists('extra-link', $sigmahEmbedVideoParamsArray) && is_array($sigmahEmbedVideoParamsArray['extra-link']) ):
            ?>
            <br />
            <a href="<?php echo $sigmahEmbedVideoParamsArray['extra-link']['url']; ?>" style="background-color:white;color:black;">
            &nbsp;<?php echo $sigmahEmbedVideoParamsArray['extra-link']['label']; ?>&nbsp;
            </a>             
            <?php
              endif;
            ?>
            </td>      
          <td style="width:160px">&nbsp;</td>  
        </tr> 
      </tbody>
    </table>  
    