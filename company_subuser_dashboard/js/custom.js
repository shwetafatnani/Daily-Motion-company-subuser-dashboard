jQuery(document).ready(function() {
  jQuery( ".ui-autocomplete-input" ).focus();
});

function videoPopupOpen() {
 jQuery('.popup-open').trigger('click');
}

function sendCommentPopupOpen() {
 jQuery('#edit-send-comment').trigger('click');
}

function skipVideo() {
 jQuery('#edit-skip').trigger('click');
}

function closeVideoPopup() {
 jQuery('.close').trigger('click');
}