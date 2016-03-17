<?php

/**
 * @file
 * Provide drush functionality for LIMS
 */
 
/**
 * Implements hook_drush_command().
 */
function lims_drush_command() {

  $items['lims-get-run'] = array(
    'description' => 'Gets information about a given run.',
    'aliases' => array('get-run'),
    'options' => array(
      'run_name' => 'All or part of the name of the run.',
      'sample_name' => 'The name of a sample in the run of interest.',
      'sample_accession' => 'The accession of a sample in the run of interest.',
      'format' => 'Defines how you want the results. One of "list" or "table"; default: table. List shows more detail.',
    ),
    'category' => 'lims',
  );

  return $items;
}

/**
 * Drush command for getting information about a run.
 */
function drush_lims_get_run() {
  $run_name = drush_get_option('run_name');
  $sample_name = drush_get_option('sample_name');
  $sample_accession = drush_get_option('sample_accession');
  
  $format = drush_get_option('format');
  if (empty($format)) $format = 'table';
  
  // If the user provided the run name then we will use that to query LIMS.
  if ($run_name) {
    $results = db_query('SELECT * FROM lims_seqrun WHERE run_name~:name', array(':name' => $run_name))->fetchAll();
    if (!$results) {
      return drush_set_error('NAME_NOT_FOUND', dt('No runs have that name.'));
    }
    lims_print_drush_run_result($results, $format);
    drush_print(sizeof($results) . ' runs have a name containing "' . $run_name . '".');
  }
  
  if ($sample_name) {
    $results = db_query(
      'SELECT run.*, sample.sample_name, sample.sample_accession, stock.name as stock_name
        FROM lims_seqrun_samples sample 
        LEFT JOIN chado.stock stock ON stock.stock_id=sample.sample_stock_id 
        LEFT JOIN lims_seqrun run ON run.samples_fid=sample.samples_fid OR run.sample_id=sample.sample_id
        WHERE stock.name~:name',
      array(':name' => $sample_name))->fetchAll();
    if (!$results) {
      return drush_set_error('SAMPLE_NAME_NOT_FOUND', dt('No runs have a sample with that name.'));
    }
    lims_print_drush_run_result($results, $format);
    drush_print(sizeof($results) . ' runs have a sample with a name containing "' . $sample_name . '".');
  }

  if ($sample_accession) {
    $results = db_query(
      'SELECT run.*, sample.sample_name, sample.sample_accession, stock.name as stock_name
        FROM lims_seqrun_samples sample 
        LEFT JOIN chado.stock stock ON stock.stock_id=sample.sample_stock_id 
        LEFT JOIN lims_seqrun run ON run.samples_fid=sample.samples_fid OR run.sample_id=sample.sample_id
        WHERE stock.uniquename=:accession',
      array(':accession' => $sample_accession))->fetchAll();
    if (!$results) {
      return drush_set_error('SAMPLE_ACCESSION_NOT_FOUND', dt('No runs have a sample with that accession.'));
    }
    lims_print_drush_run_result($results, $format);
    drush_print(sizeof($results) . ' runs have a sample with an accession of "' . $sample_accession . '".');
  }
  
  if ($format == 'table') {
    drush_print();
    drush_print('NOTE: To access these records through the web interface go to http://knowpulse.usask.ca/portal/node/[nid] after replacing [nid] with the nid of the run you are interested in.');
    drush_print();
  }
}

/**
 * Print LIMS Run results in a standardized way.
 */
function lims_print_drush_run_result($results, $format) {

  if ($format == 'list') {
    foreach ($results as $run) {
      foreach($run as $key => $value) {
        if (!empty($value)) drush_print($key . ': ' . $value);
      }
      drush_print('Web URL: http://knowpulse.usask.ca/portal/node/'.$run->nid);
      drush_print();
    }
    return TRUE;
  }
  elseif ($format == 'table') {

    $properties = array('run_name','nid','library_type','read_type','sample_name','sample_accession','stock_name');
    
    // Find longest string in each column
    $columns = [];
    foreach ($results as $row_key => $row) {
      foreach ($row as $cell_key => $cell) {
        if (in_array($cell_key, $properties)) {
          if (!isset($header[$cell_key])) $header[$cell_key] = $cell_key;
          $length = strlen($cell);
          if (empty($columns[$cell_key]) || $columns[$cell_key] < $length) {
            $columns[$cell_key] = $length;
          }
        }
      }
    }
    // Ensure the header is taken into account.
    foreach ($header as $cell_key => $cell) {
      $length = strlen($cell);
      if (empty($columns[$cell_key]) || $columns[$cell_key] < $length) {
        $columns[$cell_key] = $length;
      }
    }
 
    // Output header, padding columns;
    $rendered_header = '  |  ';
    foreach ($header as $cell_key => $cell) {
      $rendered_header .= str_pad($cell, $columns[$cell_key]) . '  |  ';
    }
    $divider .= '  +' . str_repeat('-', strlen($rendered_header)-6) . '+';
    
    drush_print($divider);
    drush_print($rendered_header);
    drush_print($divider);
    
    // Output data, padding columns
    $table = '';
    foreach ($results as $row_key => $row) {
      if (!empty($table)) $table .= PHP_EOL;
      $table .= '  |  ';
      foreach ($row as $cell_key => $cell) {
        if (in_array($cell_key, $properties)) {
          $table .= str_pad($cell, $columns[$cell_key]) . '  |  ';
        }
      }
      
    }
    drush_print($table);
    drush_print($divider);

  }
  else {
    return drush_set_error('UNKNOWN_FORMAT', dt('Unrecorgnized format "' . $format . '".'));
  }
}