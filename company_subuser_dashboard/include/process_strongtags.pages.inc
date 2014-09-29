<?php

/**
 * @File.
 */
function company_subuser_dashboard_strongtag_form($form, &$form_state) {

  $form = array();
  $form['#tree'] = TRUE;

  $form['#attached']['css'][] = drupal_get_path('module', 'company_subuser_dashboard') . '/tags_input/jquery.tagsinput.css';
  $form['#attached']['js'][] = drupal_get_path('module', 'company_subuser_dashboard') . '/tags_input/jquery.tagsinput.js';
  $form['#attached']['js'][] = drupal_get_path('module', 'company_subuser_dashboard') . '/tags_input/jquery-ui.min.js';
  $form['#attached']['css'][] = drupal_get_path('module', 'company_subuser_dashboard') . '/tags_input/jquery-ui.css';
  $form['#attached']['js'][] = drupal_get_path('module', 'company_subuser_dashboard') . '/freebase_suggest/suggest_implement.js';

  if (empty($form_state['total_count'])) {
    $form_state['total_count'] = get_all_pending_video(1);
  }

  if (empty($form_state['progress'])) {
    $form_state['progress'] = 0;
  }

  $processed = $form_state['progress'];
  $processed++;

  $form_state['current_no'] = $processed;

  if (empty($form_state['total_count'])) {
    $form['result'] = array(
      '#markup' => "<div class=\"pager_total_wrapper1\">No Video Found.</div>",
    );
    return $form;
  }

  $form['result'] = array(
    '#markup' => "<div class=\"pager_total_wrapper\"><span class=\"pager_total\">" . $processed . "</span> / <span class=\"pager_total_second\">" . count($form_state['total_count']) . "</span></div>",
  );

  $node_data = node_load($form_state['total_count'][$form_state['progress']]);
  $video_data = get_video_details(@$node_data->field_video_id[LANGUAGE_NONE][0]['value']);
  $manager_id = get_user_manager();
  $video_id = $node_data->field_video_id[$node_data->language][0]['value'];
  $video_title = $node_data->title;
  $ref_node_data = node_load($node_data->field_parent_node_reference[LANGUAGE_NONE][0]['target_id']);
  $node_comment = isset($ref_node_data->body[LANGUAGE_NONE][0]['value']) ? $ref_node_data->body[LANGUAGE_NONE][0]['value'] : '';

  ctools_include('modal');
  ctools_modal_add_js();

  $form['#attached']['js'][] = drupal_get_path('module', 'company_subuser_dashboard') . '/js/custom.js';
  $form['video_url'] = array(
    '#type' => 'markup',
    '#prefix' => '<div class="click-send">',
    '#markup' => '<!-- Modal -->
                  <div class="modal fade" id="myModal" tabindex="-1" role="dialog" aria-labelledby="myModalLabel" aria-hidden="true">
                     <div class="modal-dialog">
                        <div class="modal-content">
                           <div class="modal-header">
                              <button type="button" class="close" data-dismiss="modal" aria-hidden="true">&times;</button>
                              <h4 class="modal-title">' . $node_data->title . '</h4>
                           </div>
                           <div class="modal-body">
                              <div class="te"><iframe width="100%" height="250"  src="' . $video_data['embed_url'] . '"></iframe></div>
                           </div>
                           <div class="modal-footer">
                              <button type="button" class="btn btn-default" data-dismiss="modal">Close</button>
                           </div>
                        </div>
                        <!-- /.modal-content -->
                     </div>
                     <!-- /.modal-dialog -->
                  </div>'
  );

  $form['Send_comment'] = array(
    '#type' => 'link',
    '#title' => 'Send Message',
    '#href' => 'company_subuser_dashboard/nojs/messages/new/' . $manager_id . '/' . $video_id . '/' . $video_title,
    '#attributes' => array('class' => array('btn btn-info ctools-use-modal')),
    '#suffix' => '</div><br />',
  );

  $form['video_image'] = array(
    '#type' => 'markup',
    '#markup' => !empty($video_data['thumbnail_url']) ? theme('image', array(
          'path' => $video_data['thumbnail_url'],
          'width' => '40%',
          'height' => 'auto',
          'attributes' => array('class' => 'popup-open', 'data-toggle'=>'modal', 'data-target'=>'#myModal')
        )) : 'No Image Found',
  );

  $form["markup-video-id-{$processed}"] = array(
    '#type' => 'item',
    '#markup' => $video_id,
    '#title' => 'Video ID: ',
    '#prefix' => '<div class="strongvideo-details"><div class="strongvideo-id">',
    '#suffix' => '</div>',
  );

  $form["markup-{$processed}"] = array(
    '#type' => 'item',
    '#title' => 'Video Title: ',
    '#markup' => !empty($node_data->title) ? render($node_data->title) : ' ',
    '#prefix' => '<div class="strongvideo-title">',
    '#suffix' => '</div>',
  );

  $form["video-description-{$processed}"] = array(
    '#type' => 'item',
    '#title' => 'Video Description: ',
    '#markup' => !empty($node_data->body[LANGUAGE_NONE][0]['value']) ? $node_data->body[LANGUAGE_NONE][0]['value'] : 'No description available.',
    '#prefix' => '<div class="strongvideo-description">',
    '#suffix' => '</div></div>',
  );

  $form["video-comment-{$processed}"] = array(
    '#type' => 'textarea',
    '#title' => 'Video Comment',
    '#rows' => 2,
    '#default_value' => !empty($node_comment) ? $node_comment : 'No comment',
    '#disabled' => TRUE,
    '#prefix' => '<div class="strongvideo-comment">',
    '#suffix' => '</div>',
  );

  $form["strong_tags_added-{$processed}"] = array(
    '#type' => 'textarea',
    '#title' => 'Original Strongs Tags',
    '#disabled' => TRUE,
  );

  $form["strong_tags_tobe_add-{$processed}"] = array(
    '#type' => 'textfield',
    '#title' => 'Add Strong Tags',
    '#attributes' => array('class' => array('freebase-suggestion')),
  );

  $form["currentnode-{$processed}"] = array(
    '#type' => 'hidden',
    '#value' => $node_data->nid,
  );

  $form['skip'] = array(
    '#type' => 'submit',
    '#value' => 'SKIP',
    '#prefix' => '<div class="submit_wrapper">',
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => 'OK',
    '#suffix' => '</div>',
  );

  return $form;
}

function company_subuser_dashboard_strongtag_form_validate($form, &$form_state) {
  switch ($form_state['input']['op']) {
    case 'OK':
      $tagsadded = "strong_tags_tobe_add-{$form_state['current_no']}";
      $currentnode = "currentnode-{$form_state['current_no']}";

      if (empty($form_state['values'][$tagsadded])) {
        form_set_error("$tagsadded", t('Strong Tags added field is required.'));
      }

      if (empty($form_state['values'][$currentnode])) {
        form_set_error("", t('Video id not found.'));
      }
      break;
  }
}

function company_subuser_dashboard_strongtag_form_submit($form, &$form_state) {
  switch ($form_state['input']['op']) {
    case 'OK':
      $tagsadded = "strong_tags_tobe_add-{$form_state['current_no']}";
      $currentnode = "currentnode-{$form_state['current_no']}";
      $video_data = node_load($form_state['values'][$currentnode]);
      $tags = explode(",", $form_state['values'][$tagsadded]);
      if (!empty($tags)) {
        foreach ($tags as $keys => $value) {
          $video_data->field_strong_tags_added[LANGUAGE_NONE][$keys]['value'] = $value;
        }
      }
      $video_data->field_vm_status[LANGUAGE_NONE][0]['tid'] = 34;
      node_save_action($video_data);
      break;

    case 'SKIP':
      $currentnode = "currentnode-{$form_state['current_no']}";
      $video_data = node_load($form_state['values'][$currentnode]);
      $video_data->field_vm_status[LANGUAGE_NONE][0]['tid'] = 51;
      $video_data->field_date_of_job_skipped[LANGUAGE_NONE][0]['value'] = date("Y-m-d h:m:s");
      node_save_action($video_data);
  }

  $form_state['progress']++;
  if (count($form_state['total_count']) == $form_state['progress']) {
    drupal_set_message(t('All job processed.'));
    drupal_goto('csub-user-addstrongtags');
  }
  else {
    $form_state['rebuild'] = TRUE;
  }
}
