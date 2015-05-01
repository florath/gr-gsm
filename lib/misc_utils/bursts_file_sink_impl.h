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

#ifndef INCLUDED_GRGSM_MISC_UTILS_BURSTS_FILE_SINK_IMPL_H
#define INCLUDED_GRGSM_MISC_UTILS_BURSTS_FILE_SINK_IMPL_H

#include <grgsm/misc_utils/bursts_file_sink.h>

namespace gr {
namespace gsm {

class bursts_file_sink_impl : public bursts_file_sink {
public:
  bursts_file_sink_impl(std::string const &filename);
  ~bursts_file_sink_impl();

private:
  void bursts_process(pmt::pmt_t msg);
  int file_fd;
};
}
}

#endif
