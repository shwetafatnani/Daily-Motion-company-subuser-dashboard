(function ($) {
    Drupal.behaviors.company_subuser_dashboard = {
        attach: function (context, settings) {
            window.history.forward();
            if ($(".tagsinput").length <= 0) {
                $(".freebase-suggestion").tagsInput({
                    width: 'auto',
                    autocomplete_url: Drupal.settings.basePath + 'get_freebase_suggestion'
                }); 
            }
        }
    };
})(jQuery);