<?php
/**
 * For importing quality data for a set of samples (e.g. number of reads).
 */
class LimsSampleQualityImporter extends TripalImporter {

  /**
   * The name of this loader.  This name will be presented to the site
   * user.
   */
  public static $name = 'LIMS Sample Quality Importer';

  /**
   * The machine name for this loader. This name will be used to construct
   * the URL for the loader.
   */
  public static $machine_name = 'lims_samplequality_loader';

  /**
   * A brief description for this loader.  This description will be
   * presented to the site user.
   */
  public static $description = 'Attach quality information to LIMS records such as number of reads, basepairs and depth on a per sample basis.';

  /**
   * An array containing the extensions of allowed file types.
   */
  public static $file_types = ['txt', 'tsv'];

  /**
   * A boolean indicated whether to require an analysis for the import job.
   */
  public static $use_analysis = FALSE;

  /**
   * Provides information to the user about the file upload.  Typically this
   * may include a description of the file types allowed.
   */
  public static $upload_description = '<p>Quality information is provided on a per sample basis in a simple tab-delimited file with the following columns:</p>
    <ol>
      <li><strong>LIMS Run Name</strong>:</li>
      <li><strong>Sample Name</strong>: The name of the sample as it appears in LIMS.</li>
      <li><strong>Sample Accession</strong>: The accession of the sample as determined by LIMS.</li>
      <li><strong>Read Count</strong>: The number of reads yielded by sequencing.</li>
      <li><strong>Base Count</strong>: The total number of bases yielded by sequencing.</li>
      <li><strong>Estimated Depth</strong>: This is calculated using the total base count and dividing it by the expected number of bases in the target region. The target region will be dependent upon the library type (Whole genome, exome capture, etc).</li>
      <li><strong>Maximum Read Length</strong>: The number fo bases in the largest read sequenced.</li>
      <li><strong>N50 Value</strong>: Where contigs are considered from longest to shortest, the number of contigs in which half the assembly is contained in.</li>
      <li><strong>N50 Size</strong>: Where contigs are ordered from longest to shortest, the sequence length of the shortest contig at 50% of the total genome length.</li>
      <li><strong>Quality Note</strong>: Free text comments pertaining to the quality of the run.</li>
    </ol>';

  /**
   * Indicates the methods that the file uploader will support.
   */
  public static $methods = [
    // Allow the user to upload a file to the server.
    'file_upload' => TRUE,
    // Allow the user to provide the path on the Tripal server for the file.
    'file_local' => TRUE,
    // Allow the user to provide a remote URL for the file.
    'file_remote' => TRUE,
  ];

  /**
   * @see TripalImporter::run()
   */
  public function run() {

    // All values provided by the user in the Importer's form widgets are
    // made available to us here by the Class' arguments member variable.
    $arguments = $this->arguments['run_args'];

    // The path to the uploaded file is always made available using the
    // 'files' argument. The importer can support multiple files, therefore
    // this is an array of files, where each has a 'file_path' key specifying
    // where the file is located on the server.
    $file_path = $this->arguments['files'][0]['file_path'];

    // We want to provide a progress report to the end-user so that they:
    // 1) Recognize that the loader is not hung if running a large file, but is
    //    executing
    // 2) Provides some indicatation for how long the file will take to load.
    //
    // Here we'll get the size of the file and tell the TripalImporter how
    // many "items" we have to process (in this case bytes of the file).
    $filesize = filesize($file_path);
    $this->setTotalItems($filesize);
    $this->setItemsHandled(0);

    // Loop through each line of file.  We use the fgets function so as not
    // to load the entire file into memory but rather to iterate over each
    // line separately.
    $bytes_read = 0;
    $in_fh = fopen($file_path, "r");
    $header = explode("\t",fgets($in_fh));
    while ($line = fgets($in_fh)) {

      // Calculate how many bytes we have read from the file and let the
      // importer know how many have been processed so it can provide a
      // progress indicator.
      $bytes_read += drupal_strlen($line);
      $this->setItemsHandled($bytes_read);

      // Remove any trailing white-space from the line.
      $line = trim($line);

      // Split line on a comma into an array.  The feature name appears in the
      // first "column" of data and the property in the second.
      $cols = explode("\t", $line);

      // 1) Find the sample we are attaching quality data to.
      $run_name = $cols[0];
      $sample_name = $cols[1];
      $sample_accession = $cols[2];
      $sql = "
        SELECT s.sample_id
        FROM lims_seqrun_samples s
        INNER JOIN lims_seqrun r ON r.sample_id=s.sample_id
        WHERE r.run_name=:run
          AND s.sample_name=:name AND s.sample_accession=:accession";
      $sample_id = db_query($sql, [
        ':name' => $sample_name,
        ':accession' => $sample_accession,
        ':run' => $run_name
      ])->fetchField();
      if (!$sample_id) {
        continue;
      }

      // 2) Next lets save the quality information!
      for($i = 3; $i <= sizeof($cols); $i++) {
        if ($cols[$i]) {
          $info_label = trim($header[$i]);
          $value = trim($cols[$i]);
          $sample_info_id = db_select('lims_seqrun_samples_info', 'info')
            ->fields('info', ['sample_info_id'])
            ->condition('sample_id', $sample_id)
            ->condition('info_label', $info_label)
            ->execute()->fetchField();

          if ($sample_info_id) {
            db_update('lims_seqrun_samples_info')
              ->fields(['value' => $value])
              ->condition('sample_info_id', $sample_info_id)
              ->execute();
          }
          else {
            db_insert('lims_seqrun_samples_info')
              ->fields([
                'sample_id' => $sample_id,
                'info_label' => $info_label,
                'value' => $value,
              ])->execute();
          }
        }
      }
    }
  }
}
