(function ($) {
  Drupal.behaviors.revision_all = function (context) {
    if($('#edit-revision-all-revision-all', context).is(':checked')) {
      $('#revision-all-revision-types', context).hide();
    }

    $('#edit-revision-all-revision-all', context).change(function() {
      $('#revision-all-revision-types', context).toggle();
    });
  }
})(jQuery);
