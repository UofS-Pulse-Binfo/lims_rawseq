<?php

/**
 * @file
 * Contains views integration for lims.
 */

/**
 * Implements hook_views_data().
 */
function lims_views_data() {
  $data = array();

  // Sequencing Runs
  //---------------------------------------------  
  $data['lims_seqrun']['table']['group'] = t('Sequencing Run');
  $data ['lims_seqrun']['table']['base'] = array(
    'field' => 'run_id', // This is the identifier field for the view.
    'title' => t('Sequencing Run'),
    'help' => t('Contains "Sequencing Run" node type-specific data.'),
    'weight' => -10,
  );

  $data ['lims_seqrun']['table']['join'] = array(
    // Index this array by the table name to which this table refers.
    // 'left_field' is the primary key in the referenced table.
    // 'field' is the foreign key in this table.
    'node' => array(
      'left_field' => 'nid',
      'field' => 'nid',
    ),
  );

  // run_id
  $data['lims_seqrun']['run_id'] = array(
    'title' => t('Sequencing Run ID'),
    'help' => t('The primary key of a sequencing run.'),
  );

  // nid
  $data['lims_seqrun']['nid'] = array(
    'title' => t('Sequencing Run content'),
    'help' => t('Sequencing run content that references a node.'),
    // Define a relationship to the {node} table, so example_table views can
    // add a relationship to nodes. If you want to define a relationship the
    // other direction, use hook_views_data_alter(), or use the 'implicit' join
    // method described above.
    'relationship' => array(
      'base' => 'node', // The name of the table to join with.
      'base field' => 'nid', // The name of the field on the joined table.
      // 'field' => 'nid' -- see hook_views_data_alter(); not needed here.
      'handler' => 'views_handler_relationship',
      'label' => t('Sequencing Run Node'),
      'title' => t('Sequencing Run Node'),
      'help' => t('Join the node content to the Sequencing node.'),
    ),
  );

  // filename
  $data['lims_seqrun']['filename'] = array(
    'title' => t('Filename'),
    'help' => t('The full name of the file.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE, // This is use by the table display plugin.
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  // md5sum
  $data['lims_seqrun']['md5sum'] = array(
    'title' => t('MD5 Checksum'),
    'help' => t('The MD5 hash describing the file.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE, // This is use by the table display plugin.
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  // library_type
  $data ['lims_seqrun']['library_type'] = array(
    'title' => t('Library Type'),
    'help' => t('The method of library preparation for sequencing.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE, // This is use by the table display plugin.
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  // insert_length
  $data['lims_seqrun']['insert_length'] = array(
    'title' => t('Insert Length'),
    'help' => t('The length of the insert in base pairs.'),
    'field' => array(
      'handler' => 'views_handler_field_numeric',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  // fragment_length
  $data['lims_seqrun']['fragment_length'] = array(
    'title' => t('Fragment Length'),
    'help' => t('The length of the fragment in base pairs.'),
    'field' => array(
      'handler' => 'views_handler_field_numeric',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );


  // index_type
  $data['lims_seqrun']['index_type'] = array(
    'title' => t('Index Type'),
    'help' => t('The collection of barcodes used to identify samples.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE, // This is use by the table display plugin.
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  // technology
  $data['lims_seqrun']['technology'] = array(
    'title' => t('Technology'),
    'help' => t('The sequencing platform.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE, // This is use by the table display plugin.
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  // read_type
  $data['lims_seqrun']['read_type'] = array(
    'title' => t('Read Type'),
    'help' => t('The method used to generate the reads.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE, // This is use by the table display plugin.
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  // description
  $data['lims_seqrun']['description'] = array(
    'title' => t('Run Description'),
    'help' => t('Additional details related to this sequencing run.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE, // This is use by the table display plugin.
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  return $data;
}