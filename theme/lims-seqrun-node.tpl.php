<?php
/**
 * This template is used to render the Sequencing Run on the page.
 */
drupal_add_library('system', 'drupal.collapse');
$node = $node['#node'];

// Prepare Species for display.
$species = array();
foreach ($node->species as $s) {
  $species[] = '<i>' . $s->genus . ' ' . $s->species . '</i>';
}

// Compile data for the overview.
$overview_data = [];
$overview_data[] = ['Name', $node->seqrun->run_name];
$overview_data[] = ['Species', implode(', ', $species)];
$overview_data[] = ['Library Type', $node->seqrun->library_type];
$overview_data[] = ['Insert Length (bp)', $node->seqrun->insert_length];
$overview_data[] = ['Fragment Length (bp)', $node->seqrun->fragment_length];
$overview_data[] = ['Index Type', $node->seqrun->index_type];
$overview_data[] = ['Technology', $node->seqrun->technology];
$overview_data[] = ['Read Type', $node->seqrun->read_type];

$file_data = [];
$file_data[] = ['File', $node->seqrun->filename];
$file_data[] = ['MD5 Checksum', $node->seqrun->md5sum];

// Compile data for samples.
// -- If we have a single sample then we want to show a simple list...
$single_sample = (sizeof($node->samples) == 1) ? TRUE: FALSE;
if ($single_sample) {

  $sample = $node->samples[0];

  $entity_id = FALSE;
  if (isset($sample->sample_stock_id) AND $sample->sample_stock_id > 0) {
    $entity_id = chado_get_record_entity_by_table('stock', $sample->sample_stock_id);
  }
  $sample_accession = ($entity_id) ? l($sample->sample_accession, 'bio_data/'.$entity_id, array('attributes' => array('target' => '_blank'))) : $sample->sample_accession;


  $sample_data = [];
  $sample_data[] = ['Name', $sample->sample_name];
  $sample_data[] = ['Accession', $sample_accession];
  $sample_data[] = ['Description', $sample->sample_description];

  $sample_quality_data = [];
  foreach ($sample->quality_info as $label => $value) {
    $sample_quality_data[] = [$label, $value];
  }
}
// -- If we have a multi-sample run then we want to show a table...
elseif (!$single_sample AND !empty($node->samples)) {

  $sample_data = [];
  $have_quality = FALSE;
  foreach ($node->samples as $sample) {

    $entity_id = FALSE;
    if (isset($sample->sample_stock_id) AND $sample->sample_stock_id > 0) {
      $entity_id = chado_get_record_entity_by_table('stock', $sample->sample_stock_id);
    }
    $sample_accession = ($entity_id) ? l($sample->sample_accession, 'bio_data/'.$entity_id, array('attributes' => array('target' => '_blank'))) : $sample->sample_accession;

    $sample_data[] = array(
      $sample->index_id,
      $sample->index_seq,
      $sample->sample_name,
      $sample_accession,
      $sample->plate_well,
      $sample->sample_description
    );


    if ($sample->quality_info) {
      $have_quality = TRUE;
      $sample_quality_data[] = array_merge(
        ['Name' => $sample->sample_name, 'Accession' => $sample_accession],
        $sample->quality_info
      );
      foreach ($sample->quality_info as $label => $value) {
        $sample_quality_header[$label] = $label;
      }
    }
    else {
      $sample_quality_data[] = [
        'Name' => $sample->sample_name,
        'Accession' => $sample_accession
      ];
    }
  }
}
?>

<style>
/* Theming for the link to download indices */
a.indices-link {
  float:right;
  margin-top: 17px;
}
h2.samples-table-title {
  margin-top: 0;
}
.vertical-header th {
  width: 20%;
}

</style>

<!-- OVERVIEW -->
<fieldset id="lims-overview" class="collapsible form-wrapper">
  <legend><span class="fieldset-legend">
    <a class="fieldset-title" href="#"><span class="fieldset-legend-prefix element-invisible">Hide</span> Overview</a><span class="summary"></span></span>
  </legend>
  <div class="fieldset-wrapper">

    <table id="overview" class="vertical-header">
      <caption>Basic Information</caption>
      <tbody>
        <?php foreach ($overview_data as $row) :
            if (!empty($row[1])) : ?>
              <tr><th><?php print $row[0]?></th><td><?php print $row[1]?></td></tr>
        <?php endif; endforeach; ?>
      </tbody>
    </table>

    <table id="file" class="vertical-header">
      <caption>File Information</caption>
      <tbody>
        <?php foreach ($file_data as $row) :
          if (!empty($row[1])) : ?>
          <tr><th><?php print $row[0]?></th><td><?php print $row[1]?></td></tr>
        <?php else : ?>
          <tr><th><?php print $row[0]?></th><td><span class="empty">Not available</span></td></tr>
        <?php endif; endforeach; ?>
      </tbody>
    </table>

    <p><?php print $node->seqrun->description; ?></p>

  </div>
</fieldset>

<!-- SAMPLES -->
<fieldset id="lims-samples-list" class="collapsible collapsed form-wrapper">
  <legend><span class="fieldset-legend">
    <a class="fieldset-title" href="#"><span class="fieldset-legend-prefix element-invisible">Hide</span> Sample(s) List</a><span class="summary"></span></span>
  </legend>
  <div class="fieldset-wrapper">

    <?php if ($single_sample) : ?>

      <table id="sample" class="vertical-header single-sample">
        <tbody>
          <?php foreach ($sample_data as $row) :
              if (!empty($row[1])) : ?>
                <tr><th><?php print $row[0]?></th><td><?php print $row[1]?></td></tr>
          <?php endif; endforeach; ?>
        </tbody>
      </table>

    <?php elseif (empty($samples)): ?>

      <br />
      <h3 style="margin:0;padding:0;color:red">No samples uploaded for this Sequencing Run.</h3>
      <div>Please Edit this page and upload a tab-delimited file adhering to the specifications mentioned under "Samples Details"</div>

    <?php else: ?>

      <table id="sample" class="horizontal-header multi-sample">
        <thead>
          <tr><th colspan="2">Index</th><th colspan="4">Sample</th></tr>
          <tr>
            <th>ID</th><th>Sequence</th>
            <th>Name</th><th>Accession</th><th>Plate Well</th><th>Description</th></th>
        </thead>
        <tbody>
          <?php foreach ($sample_data as $row) :?>
            <tr>
              <?php foreach ($row as $cell) : ?>
                <td><?php print $cell;?></td>
              <?php endforeach; ?>
            </tr>
          <?php endforeach; ?>
        </tbody>
      </table>

    <?php endif; ?>

  </div>
</fieldset>

<!-- QUALITY -->
<fieldset id="lims-sample-quality" class="collapsible collapsed form-wrapper">
  <legend><span class="fieldset-legend">
    <a class="fieldset-title" href="#"><span class="fieldset-legend-prefix element-invisible">Hide</span> Quality Statistics</a><span class="summary"></span></span>
  </legend>
  <div class="fieldset-wrapper">

    <?php if ($single_sample AND $have_quality) : ?>

      <table id="quality" class="vertical-header single-sample">
        <tbody>
          <?php foreach ($sample_quality_data as $row) :
              if (!empty($row[1])) : ?>
                <tr><th><?php print $row[0]?></th><td><?php print $row[1]?></td></tr>
          <?php endif; endforeach; ?>
        </tbody>
      </table>

    <?php elseif (!$have_quality): ?>

      <br />
      <h3 style="margin:0;padding:0;color:red">No quality data available for this Sequencing Run.</h3>
      <div>Please upload it using the LIMS Sample Quality Tripal Importer</div>

    <?php else: ?>

      <table id="quality" class="horizontal-header multi-sample">
        <thead>
          <tr><th colspan="2">Sample</th><th colspan="<?php print sizeof($sample_quality_header)+1;?>">Quality Statistics</th></tr>
          <tr>
            <th>Name</th><th>Accession</th>
            <?php foreach($sample_quality_header as $header_cell): ?>
              <th><?php print $header_cell;?></th>
            <?php endforeach; ?>
          </tr>
        </thead>
        <tbody>
          <?php foreach ($sample_quality_data as $row) :?>
            <tr>
              <td><?php print $row['Name'];?></td>
              <td><?php print $row['Accession'];?></td>
              <?php foreach ($sample_quality_header as $label) :
                if (!isset($row[$label])) : ?>
                <td></td>
              <?php else: ?>
                <td><?php print $row[$label];?></td>
              <?php endif; endforeach; ?>
            </tr>
          <?php endforeach; ?>
        </tbody>
      </table>

    <?php endif; ?>

  </div>
</fieldset>

<?php

/******

// Samples Table
//-------------------------
if (!empty($node->samples)) {
  // CASE 1: Single Sample with no index.
  if (sizeof($node->samples == 1) AND !isset($node->samples[0]->index_id)) {
    $rows = array();

    $sample = $node->samples[0];

    $sample_name = (isset($sample->sample_stock_nid)) ? l($sample->sample_name, 'node/'.$sample->sample_stock_nid, array('attributes' => array('target' => '_blank'))) : $sample->sample_name;
    $sample_accession = (isset($sample->sample_stock_nid)) ? l($sample->sample_accession, 'node/'.$sample->sample_stock_nid, array('attributes' => array('target' => '_blank'))) : $sample->sample_accession;

    $rows[] = array(
      array( 'data' => 'Name', 'header' => TRUE, 'width' => '20%' ),
      $sample_name
    );
    $rows[] = array(
      array( 'data' => 'Accession', 'header' => TRUE, 'width' => '20%' ),
      $sample_accession
    );
    $rows[] = array(
      array( 'data' => 'Description', 'header' => TRUE, 'width' => '20%' ),
      $sample->sample_description
    );

    print '<br /><h2>Sample</h2>' . theme('table',array('header' => array(), 'rows' => $rows));

    // Now Quality table.
    if (isset($sample->quality_info)) {
      $rows = [];
      foreach($sample->quality_info as $label => $value) {
        $rows[] = array(
          array( 'data' => $label, 'header' => TRUE, 'width' => '20%' ),
          $value
        );
      }
      print '<br /><h2>Quality</h2>' . theme('table',array('header' => array(), 'rows' => $rows));
    }
  }
  // CASE 2: Multiple samples.
  elseif (sizeof($node->samples) > 0) {
    $header = array(
      array('data' => 'Index', 'colspan' => 2, 'style' => 'text-align:center;'),
      array('data' => 'Sample', 'colspan' => 4, 'style' => 'text-align:center;'),
    );
    $rows = array();
    // Second header.
    $rows[] = array(
      array( 'data' => 'ID', 'header' => TRUE),
      array( 'data' => 'Sequence', 'header' => TRUE),
      array( 'data' => 'Name', 'header' => TRUE),
      array( 'data' => 'Accession', 'header' => TRUE),
      array( 'data' => 'Plate Well', 'header' => TRUE),
      array( 'data' => 'Description', 'header' => TRUE),
    );
    foreach ($node->samples as $sample) {

      $sample_name = (isset($sample->sample_stock_nid)) ? l($sample->sample_name, 'node/'.$sample->sample_stock_nid, array('attributes' => array('target' => '_blank'))) : $sample->sample_name;
      $sample_accession = (isset($sample->sample_stock_nid)) ? l($sample->sample_accession, 'node/'.$sample->sample_stock_nid, array('attributes' => array('target' => '_blank'))) : $sample->sample_accession;

      $rows[] = array(
        $sample->index_id,
        $sample->index_seq,
        $sample_name,
        $sample_accession,
        $sample->plate_well,
        $sample->sample_description
      );
    }
    print '<br />';
    print l('Export Indices', 'node/' . $node->nid . '/download-index', array('attributes' => array('class' => array('indices-link'))));
    print '<h2 class="samples-table-title">Samples</h2>';
    print theme('table',array('header' => $header, 'rows' => $rows));
  }
}
else {
  print '<br /><h3 style="margin:0;padding:0;color:red">No samples uploaded for this Sequencing Run.</h3><div>Please Edit this page and upload a tab-delimited file adhering to the specifications mentioned under "Samples Details"</div>';
}
*/
