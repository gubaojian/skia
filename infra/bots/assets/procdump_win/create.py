#!/usr/bin/env python
#
# Copyright 2018 Google Inc.
#
# Use of this source code is governed by a BSD-style license that can be
# found in the LICENSE file.


"""Create the asset."""


import argparse
import common
import subprocess
import utils


# Download URL can be found on this page:
# https://docs.microsoft.com/en-us/sysinternals/downloads/procdump
PROCDUMP_URL = 'https://download.sysinternals.com/files/Procdump.zip'


def create_asset(target_dir):
  """Create the asset."""
  with utils.tmp_dir():
    subprocess.check_call(["curl", PROCDUMP_URL, "-o", "procdump.zip"])
    subprocess.check_call(["unzip", "procdump.zip", "-d", target_dir])


def main():
  parser = argparse.ArgumentParser()
  parser.add_argument('--target_dir', '-t', required=True)
  args = parser.parse_args()
  create_asset(args.target_dir)


if __name__ == '__main__':
  main()
