<?php
/**
 * This template is used to render the Sequencing Run on the page.
 */
$node = $node['#node'];
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

</style>

<?php
// Prepare Species for display.
$species = array();
foreach ($node->species as $s) {
  $species[] = '<i>' . $s->genus . ' ' . $s->species . '</i>';
}

// Main Table
//-------------------------
$rows = array();
$rows[] = array(
  array( 'data' => 'Name', 'header' => TRUE, 'width' => '20%' ),
  $node->seqrun->run_name
);
$rows[] = array(
  array( 'data' => 'Species', 'header' => TRUE, 'width' => '20%' ),
  implode(', ',$species)
);
$rows[] = array(
  array( 'data' => 'Library Type', 'header' => TRUE, 'width' => '20%' ),
  $node->seqrun->library_type
);
if (!empty($node->seqrun->insert_length)) {
  $rows[] = array(
    array( 'data' => 'Insert Length', 'header' => TRUE, 'width' => '20%' ),
    $node->seqrun->insert_length . ' bp'
  );
}
if (!empty($node->seqrun->fragment_length)) {
  $rows[] = array(
    array( 'data' => 'Fragment Length', 'header' => TRUE, 'width' => '20%' ),
    $node->seqrun->fragment_length . ' bp'
  );
}
$rows[] = array(
  array( 'data' => 'Index Type', 'header' => TRUE, 'width' => '20%' ),
  $node->seqrun->index_type
);
$rows[] = array(
  array( 'data' => 'Technology', 'header' => TRUE, 'width' => '20%' ),
  $node->seqrun->technology
);
$rows[] = array(
  array( 'data' => 'Read Type', 'header' => TRUE, 'width' => '20%' ),
  $node->seqrun->read_type
);

print theme('table',array('header' => array(), 'rows' => $rows));

// Short File table
//-------------------------
$rows = array();
$rows[] = array(
  array( 'data' => 'File', 'header' => TRUE, 'width' => '20%' ),
  $node->seqrun->filename
);
$rows[] = array(
  array( 'data' => 'MD5 Checksum', 'header' => TRUE, 'width' => '20%' ),
  $node->seqrun->md5sum
);

print theme('table',array('header' => array(), 'rows' => $rows));

print '<br /><p>'.$node->seqrun->description.'</p>';

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
