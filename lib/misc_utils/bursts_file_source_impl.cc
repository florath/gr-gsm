/* -*- c++ -*- */
/* @file
 * @author Andre Florath <andre@florath.net>
 * @section LICENSE
 *
 * Gr-gsm is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 *
 * Gr-gsm is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with gr-gsm; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street,
 * Boston, MA 02110-1301, USA.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <gnuradio/io_signature.h>
#include <grgsm/gsmtap.h>
#include "misc_utils/bursts_file_source_impl.h"
#include "receiver/gsm_constants.h"
#include <sstream>
#include <iomanip>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

namespace gr {
namespace gsm {

void bursts_file_source_impl::bursts_process() {
  if (file_fd == -1) {
    return;
  }
  std::cout << "Process bursts" << std::endl;

  char buf[198];
  while (true) {
    ssize_t const rres(::read(file_fd, buf, 197));
    if (rres == 0) {
      std::cout << "Read EOF" << std::endl;
      return;
    }

    if (rres != 197) {
      std::cout << "Read Invalid Byte Cnt" << std::endl;
      return;
    }

    boost::scoped_ptr<gsmtap_hdr> tap_header(new gsmtap_hdr());
    tap_header->version = atoi(buf + 0);
    tap_header->hdr_len = atoi(buf + 2);
    tap_header->type = atoi(buf + 4);
    tap_header->timeslot = atoi(buf + 6);
    tap_header->arfcn = atoi(buf + 8);
    tap_header->signal_dbm = atoi(buf + 14);
    tap_header->snr_db = atoi(buf + 19);
    tap_header->frame_number = atol(buf + 24);
    tap_header->sub_type = atoi(buf + 32);
    tap_header->antenna_nr = atoi(buf + 36);
    tap_header->sub_slot = atoi(buf + 40);
    tap_header->res = atoi(buf + 44);

    int8_t burst_buf[BURST_SIZE];
    for (int i(48); i < 196; ++i) {
      if (buf[i] == '0') {
        burst_buf[i - 48] = 0;
      } else {
        burst_buf[i - 48] = 1;
      }
    }
    std::cout << "Sending burst: " << tap_header->frame_number << std::endl;
    int8_t header_plus_burst[sizeof(gsmtap_hdr) + BURST_SIZE];
    memcpy(header_plus_burst, tap_header.get(), sizeof(gsmtap_hdr));
    memcpy(header_plus_burst + sizeof(gsmtap_hdr), burst_buf, BURST_SIZE);
    pmt::pmt_t blob_header_plus_burst =
        pmt::make_blob(header_plus_burst, sizeof(gsmtap_hdr) + BURST_SIZE);
    message_port_pub(pmt::mp("bursts"), blob_header_plus_burst);
  }

  return;

  abort();

#if 0
  pmt::pmt_t const header_plus_burst = pmt::cdr(msg);

  gsmtap_hdr const *const header =
      (gsmtap_hdr *)pmt::blob_data(header_plus_burst);
  int8_t const *const burst =
      (int8_t *)(pmt::blob_data(header_plus_burst)) + sizeof(gsmtap_hdr);
  size_t const burst_len =
      pmt::blob_length(header_plus_burst) - sizeof(gsmtap_hdr);

  std::stringstream out;
  out << std::setw(1) << (uint16_t)header->version << "," << std::setw(1)
      << (uint16_t)header->hdr_len << "," << std::setw(1)
      << (uint16_t)header->type << "," << std::setw(1)
      << (uint16_t)header->timeslot << "," << std::setw(5)
      << be16toh(header->arfcn) << "," << std::setw(4)
      << (int16_t)header->signal_dbm << "," << std::setw(4)
      << (int16_t)header->snr_db << "," << std::setw(7)
      << be32toh(header->frame_number) << "," << std::setw(3)
      << (uint16_t)header->sub_type << "," << std::setw(3)
      << (uint16_t)header->antenna_nr << "," << std::setw(3)
      << (uint16_t)header->sub_slot << "," << std::setw(3)
      << (uint16_t)header->res << ",";
  for (int i(0); i < burst_len; ++i) {
    out << std::setw(1) << static_cast<int>(burst[i]);
  }
  out << std::endl;

  std::string::size_type const slen(out.str().length());

  ssize_t const wres(::write(file_fd, out.str().c_str(), slen));
  if (slen != wres) {
    perror("bursts_file_source:write error");
    ::close(file_fd);
    file_fd = -1;
  }
#endif
}

bursts_file_source::sptr bursts_file_source::make(std::string const &filename) {
  return gnuradio::get_initial_sptr(new bursts_file_source_impl(filename));
}

/*
 * The private constructor
 */
bursts_file_source_impl::bursts_file_source_impl(std::string const &filename)
    : gr::block("bursts_file_source", gr::io_signature::make(0, 0, 0),
                gr::io_signature::make(0, 0, 0)),
      file_fd(open(filename.c_str(), O_RDONLY | O_NOCTTY)) {

  if (file_fd == -1) {
    perror("bursts_file_source:open file error");
  }

  message_port_register_out(pmt::mp("bursts"));

  //  bursts_process();
}

/*
 * Our virtual destructor.
 */
bursts_file_source_impl::~bursts_file_source_impl() {
  if (file_fd != -1) {
    ::close(file_fd);
  }
}

#if 0
int bursts_file_source_impl::work(int noutput_items,
                                  gr_vector_const_void_star &input_items,
                                  gr_vector_void_star &output_items) {
  std::cout << "bursts_file_source_impl::work noutput_items:" << noutput_items
            << std::endl;
  return 0;
}
#endif
}
}
