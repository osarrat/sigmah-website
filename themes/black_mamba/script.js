// $Id: script.js,v 1.1 2009/06/12 20:34:01 giorgio79 Exp $ 
Drupal.behaviors.tntIEFixes = function (context) {
  // IE6 & less-specific functions
  // Add hover class to primary menu li elements on hover
  if ($.browser.msie && ($.browser.version < 7)) {
    $('#primary-menu li').hover(function() {
      $(this).addClass('hover');
      }, function() {
        $(this).removeClass('hover');
    });
  };
};
