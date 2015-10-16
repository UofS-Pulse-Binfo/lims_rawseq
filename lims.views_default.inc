<?php
/**
 * @file
 * Defines default views for lims.
 */

/**
 * Implements hook_views_default_views().
 */
function lims_views_default_views() {

  // Sequencing Runs
  //---------------------------------------------
  $view = new view();
  $view->name = 'raw_sequence_search';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'lims_seqrun';
  $view->human_name = 'Raw Sequence Search';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Raw Sequence Search';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'none';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '50';
  $handler->display->display_options['style_plugin'] = 'table';
  /* Relationship: Sequencing Run: Sequencing Run Node */
  $handler->display->display_options['relationships']['nid']['id'] = 'nid';
  $handler->display->display_options['relationships']['nid']['table'] = 'lims_seqrun';
  $handler->display->display_options['relationships']['nid']['field'] = 'nid';
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['relationship'] = 'nid';
  $handler->display->display_options['fields']['title']['label'] = 'Name';
  /* Field: Sequencing Run: Library Type */
  $handler->display->display_options['fields']['library_type']['id'] = 'library_type';
  $handler->display->display_options['fields']['library_type']['table'] = 'lims_seqrun';
  $handler->display->display_options['fields']['library_type']['field'] = 'library_type';
  /* Field: Sequencing Run: Index Type */
  $handler->display->display_options['fields']['index_type']['id'] = 'index_type';
  $handler->display->display_options['fields']['index_type']['table'] = 'lims_seqrun';
  $handler->display->display_options['fields']['index_type']['field'] = 'index_type';
  /* Field: Sequencing Run: Technology */
  $handler->display->display_options['fields']['technology']['id'] = 'technology';
  $handler->display->display_options['fields']['technology']['table'] = 'lims_seqrun';
  $handler->display->display_options['fields']['technology']['field'] = 'technology';
  /* Field: Sequencing Run: Read Type */
  $handler->display->display_options['fields']['read_type']['id'] = 'read_type';
  $handler->display->display_options['fields']['read_type']['table'] = 'lims_seqrun';
  $handler->display->display_options['fields']['read_type']['field'] = 'read_type';
  /* Sort criterion: Sequencing Run: Library Type */
  $handler->display->display_options['sorts']['library_type']['id'] = 'library_type';
  $handler->display->display_options['sorts']['library_type']['table'] = 'lims_seqrun';
  $handler->display->display_options['sorts']['library_type']['field'] = 'library_type';
  /* Sort criterion: Sequencing Run: Technology */
  $handler->display->display_options['sorts']['technology']['id'] = 'technology';
  $handler->display->display_options['sorts']['technology']['table'] = 'lims_seqrun';
  $handler->display->display_options['sorts']['technology']['field'] = 'technology';
  /* Filter criterion: Sequencing Run: Index Type */
  $handler->display->display_options['filters']['index_type']['id'] = 'index_type';
  $handler->display->display_options['filters']['index_type']['table'] = 'lims_seqrun';
  $handler->display->display_options['filters']['index_type']['field'] = 'index_type';
  $handler->display->display_options['filters']['index_type']['group'] = 1;
  $handler->display->display_options['filters']['index_type']['exposed'] = TRUE;
  $handler->display->display_options['filters']['index_type']['expose']['operator_id'] = 'index_type_op';
  $handler->display->display_options['filters']['index_type']['expose']['label'] = 'Index Type';
  $handler->display->display_options['filters']['index_type']['expose']['description'] = 'The collection of barcodes used to identify samples.';
  $handler->display->display_options['filters']['index_type']['expose']['operator'] = 'index_type_op';
  $handler->display->display_options['filters']['index_type']['expose']['identifier'] = 'index_type';
  $handler->display->display_options['filters']['index_type']['expose']['multiple'] = TRUE;
  $handler->display->display_options['filters']['index_type']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    4 => 0,
    5 => 0,
    3 => 0,
    6 => 0,
  );
  $handler->display->display_options['filters']['index_type']['is_grouped'] = TRUE;
  $handler->display->display_options['filters']['index_type']['group_info']['label'] = 'Index Type';
  $handler->display->display_options['filters']['index_type']['group_info']['description'] = 'The collection of barcodes used to identify samples.';
  $handler->display->display_options['filters']['index_type']['group_info']['identifier'] = 'index_type';
  $handler->display->display_options['filters']['index_type']['group_info']['multiple'] = TRUE;
  $handler->display->display_options['filters']['index_type']['group_info']['group_items'] = array(
    1 => array(
      'title' => 'None',
      'operator' => '=',
      'value' => 'None',
    ),
    2 => array(
      'title' => 'Illumina indexing',
      'operator' => '=',
      'value' => 'Illumina indexing',
    ),
    3 => array(
      'title' => 'GBS 96-plex',
      'operator' => '=',
      'value' => 'GBS 96-plex',
    ),
    4 => array(
      'title' => 'Custom',
      'operator' => '=',
      'value' => 'Custom',
    ),
    5 => array(
      'title' => 'Other',
      'operator' => '=',
      'value' => 'Other',
    ),
  );
  /* Filter criterion: Sequencing Run: Library Type */
  $handler->display->display_options['filters']['library_type']['id'] = 'library_type';
  $handler->display->display_options['filters']['library_type']['table'] = 'lims_seqrun';
  $handler->display->display_options['filters']['library_type']['field'] = 'library_type';
  $handler->display->display_options['filters']['library_type']['group'] = 1;
  $handler->display->display_options['filters']['library_type']['exposed'] = TRUE;
  $handler->display->display_options['filters']['library_type']['expose']['operator_id'] = 'library_type_op';
  $handler->display->display_options['filters']['library_type']['expose']['label'] = 'Library Type';
  $handler->display->display_options['filters']['library_type']['expose']['description'] = 'The method of library preparation for sequencing.';
  $handler->display->display_options['filters']['library_type']['expose']['operator'] = 'library_type_op';
  $handler->display->display_options['filters']['library_type']['expose']['identifier'] = 'library_type';
  $handler->display->display_options['filters']['library_type']['expose']['multiple'] = TRUE;
  $handler->display->display_options['filters']['library_type']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    4 => 0,
    5 => 0,
    3 => 0,
    6 => 0,
  );
  $handler->display->display_options['filters']['library_type']['is_grouped'] = TRUE;
  $handler->display->display_options['filters']['library_type']['group_info']['label'] = 'Library Type';
  $handler->display->display_options['filters']['library_type']['group_info']['description'] = 'The method of library preparation for sequencing.';
  $handler->display->display_options['filters']['library_type']['group_info']['identifier'] = 'library_type';
  $handler->display->display_options['filters']['library_type']['group_info']['multiple'] = TRUE;
  $handler->display->display_options['filters']['library_type']['group_info']['group_items'] = array(
    1 => array(
      'title' => 'Genomic',
      'operator' => '=',
      'value' => 'Genomic',
    ),
    2 => array(
      'title' => 'RNA-seq',
      'operator' => '=',
      'value' => 'RNA-seq',
    ),
    3 => array(
      'title' => '3\' transcript',
      'operator' => '=',
      'value' => '3\' transcript',
    ),
    4 => array(
      'title' => 'Single-enzyme GBS',
      'operator' => '=',
      'value' => 'Single-enzyme GBS',
    ),
    5 => array(
      'title' => 'Dual-enzyme GBS',
      'operator' => '=',
      'value' => 'Dual-enzyme GBS',
    ),
    6 => array(
      'title' => 'ChIPseq',
      'operator' => '=',
      'value' => 'ChIPseq',
    ),
    7 => array(
      'title' => 'Exome capture',
      'operator' => '=',
      'value' => 'Exome capture',
    ),
  );
  /* Filter criterion: Sequencing Run: Technology */
  $handler->display->display_options['filters']['technology']['id'] = 'technology';
  $handler->display->display_options['filters']['technology']['table'] = 'lims_seqrun';
  $handler->display->display_options['filters']['technology']['field'] = 'technology';
  $handler->display->display_options['filters']['technology']['group'] = 1;
  $handler->display->display_options['filters']['technology']['exposed'] = TRUE;
  $handler->display->display_options['filters']['technology']['expose']['operator_id'] = 'technology_op';
  $handler->display->display_options['filters']['technology']['expose']['label'] = 'Technology';
  $handler->display->display_options['filters']['technology']['expose']['description'] = 'The sequencing platform.';
  $handler->display->display_options['filters']['technology']['expose']['operator'] = 'technology_op';
  $handler->display->display_options['filters']['technology']['expose']['identifier'] = 'technology';
  $handler->display->display_options['filters']['technology']['expose']['multiple'] = TRUE;
  $handler->display->display_options['filters']['technology']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    4 => 0,
    5 => 0,
    3 => 0,
    6 => 0,
  );
  $handler->display->display_options['filters']['technology']['is_grouped'] = TRUE;
  $handler->display->display_options['filters']['technology']['group_info']['label'] = 'Technology';
  $handler->display->display_options['filters']['technology']['group_info']['description'] = 'The sequencing platform.';
  $handler->display->display_options['filters']['technology']['group_info']['identifier'] = 'technology';
  $handler->display->display_options['filters']['technology']['group_info']['multiple'] = TRUE;
  $handler->display->display_options['filters']['technology']['group_info']['group_items'] = array(
    1 => array(
      'title' => 'Sanger',
      'operator' => '=',
      'value' => 'Sanger',
    ),
    2 => array(
      'title' => 'Illumina GAIIx',
      'operator' => '=',
      'value' => 'Illumina GAIIx',
    ),
    3 => array(
      'title' => 'Illumina HiSeq',
      'operator' => '=',
      'value' => 'Illumina HiSeq',
    ),
    4 => array(
      'title' => 'Illumina MiSeq',
      'operator' => '=',
      'value' => 'Illumina MiSeq',
    ),
    5 => array(
      'title' => 'Roche 454',
      'operator' => '=',
      'value' => 'Roche 454',
    ),
    6 => array(
      'title' => 'PacBio RS',
      'operator' => '=',
      'value' => 'PacBio RS',
    ),
    7 => array(
      'title' => 'Oxford Nanopore',
      'operator' => '=',
      'value' => 'Oxford Nanopore',
    ),
  );
  /* Filter criterion: Sequencing Run: Read Type */
  $handler->display->display_options['filters']['read_type']['id'] = 'read_type';
  $handler->display->display_options['filters']['read_type']['table'] = 'lims_seqrun';
  $handler->display->display_options['filters']['read_type']['field'] = 'read_type';
  $handler->display->display_options['filters']['read_type']['group'] = 1;
  $handler->display->display_options['filters']['read_type']['exposed'] = TRUE;
  $handler->display->display_options['filters']['read_type']['expose']['operator_id'] = 'read_type_op';
  $handler->display->display_options['filters']['read_type']['expose']['label'] = 'Read Type';
  $handler->display->display_options['filters']['read_type']['expose']['description'] = 'The method used to generate the reads.';
  $handler->display->display_options['filters']['read_type']['expose']['operator'] = 'read_type_op';
  $handler->display->display_options['filters']['read_type']['expose']['identifier'] = 'read_type';
  $handler->display->display_options['filters']['read_type']['expose']['multiple'] = TRUE;
  $handler->display->display_options['filters']['read_type']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    4 => 0,
    5 => 0,
    3 => 0,
    6 => 0,
  );
  $handler->display->display_options['filters']['read_type']['is_grouped'] = TRUE;
  $handler->display->display_options['filters']['read_type']['group_info']['label'] = 'Read Type';
  $handler->display->display_options['filters']['read_type']['group_info']['description'] = 'The method used to generate the reads.';
  $handler->display->display_options['filters']['read_type']['group_info']['identifier'] = 'read_type';
  $handler->display->display_options['filters']['read_type']['group_info']['multiple'] = TRUE;
  $handler->display->display_options['filters']['read_type']['group_info']['group_items'] = array(
    1 => array(
      'title' => 'Single-end',
      'operator' => '=',
      'value' => 'Single-end',
    ),
    2 => array(
      'title' => 'Paired-end',
      'operator' => '=',
      'value' => 'Paired-end',
    ),
    3 => array(
      'title' => 'Mate Pair (FR)',
      'operator' => '=',
      'value' => 'Mate Pair (FR)',
    ),
    4 => array(
      'title' => 'Mate Pair (RF)',
      'operator' => '=',
      'value' => 'Mate Pair (RF)',
    ),
    5 => array(
      'title' => 'Other',
      'operator' => '=',
      'value' => 'Other',
    ),
  );

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->display->display_options['path'] = 'search/sequences/raw';
  $handler->display->display_options['menu']['type'] = 'normal';
  $handler->display->display_options['menu']['title'] = 'Raw Sequence Search';
  $handler->display->display_options['menu']['weight'] = '0';
  $handler->display->display_options['menu']['name'] = 'menu-bioinformatics-resources';
  $handler->display->display_options['menu']['context'] = 0;
  $handler->display->display_options['menu']['context_only_inline'] = 0;

  $views[$view->name] = $view;

  return $views;
}