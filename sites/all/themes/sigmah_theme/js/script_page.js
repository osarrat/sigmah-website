// JavaScript Document
jQuery(document).ready(function(){
 
 id=$('a.select').attr('id');	
 $('#sub_li_'+id).css('display', 'block');
  $('#li_'+id).addClass('hover');

$('.hassub_menu').hover(function() {
		
		mouseOverMenu($(this), 'sub_'+$(this).attr('id'));
		
	},function() {


		});

$('.main_menu').mouseout(function() { 
								  
								  mouseOut(); 
							   });

$('.sub_menu').hover(function() {
							  
							  mouseOverItem();
							  },function() {
								
							mouseOut();  
								  
							  });

});


var oldItem = null;
var oldMenu = null;
var timer = null;
var _menu = null;
 
/* Fonction appelee lors du mouseover sur le menu */
function mouseOverMenu(menu, idItem)
{
 /* on arrete le timer declenche lors d un onmouseout */
 if (timer!=null)
 {
    clearTimeout(timer);
    timer = null;
 }
 
 $('.sub_menu').css('display', 'none');
 $('.hassub_menu').not(menu).removeClass('hover');
 _menu=menu;
 _menu.addClass('hover');

 
 
 /* on masque le div vide */

 
 /* si necessaire, on masque le sous-menu precedemmment affiche */
 if (oldItem!=null && oldItem!=idItem)
 {
    
	$('#'+oldItem).css('display','none');
    oldItem = null;
 }
 
 /* on affiche le sous-menu idItem */


						   
						 
 $('#'+idItem).css('display','block');
 
 
 /* on redonne le style par defaut de l ancien menu selectionne */
 if (oldMenu!=null && oldMenu!=menu)
 {
    oldMenu.className = "styleOutMenu";
    oldMenu = null;
 }
 
 /* style du menu selectionne */
 menu.className = "styleOverMenu";
 
 
 /* memorisation du choix de l utilisateur */
 oldItem = idItem;
 oldMenu = menu;
}
 
/* Fonction appelée lors du mouseover sur un element du sous-menu */
/* On arrete le chrono */
function mouseOverItem()
{
 if (timer!=null)
 {
    clearTimeout(timer);
    timer = null;
 }
}
 
/* Fonction appelee a la fin du delai indique dans la methode setTimeout */
/* On masque les sous-menu et on redonne aux elements leur parametres par defaut */
function mouseOutT()
{
 if (oldItem!=null)
 {
    $('#'+oldItem).css('display','none');
    oldItem = null;
 }
 
 if (oldMenu!=null)
 {
    oldMenu.className = "styleOutMenu";
    oldMenu = null;
 }
 

 if(_menu) {_menu.removeClass('hover');}
 $('#sub_li_'+id).css('display', 'block');
  $('#li_'+id).addClass('hover');

 
 timer = null;
}
 
/* declenchement d un timer lors du mouseout sur les elements du menu ou des sous-menus */
/* le timer est arrete si un evenement onmouseover a lieu */
function mouseOut()
{
	
  if (timer==null)
  {
    timer = setTimeout("mouseOutT()","1000");
  }
}